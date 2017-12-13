#include "stdafx.h"

#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "Stream.h"

#include "ErrorCode.h"
#include "UserProtocol.h"
#include "USER_PT_Structure.h"
#include "USER_PT_ReadPacket.h"
#include "USER_PT_WritePacket.h"

#include "DoubleBattleProtocol.h"
#include "DOUBLE_BATTLE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_ReadPacket.h"
#include "DOUBLE_BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

#include "BattleField.h"

CRoomManager::CRoomManager(VOID)
{
}

CRoomManager::~CRoomManager(VOID)
{
}

// ��ʼ������
// ͨ�����������󷿼���
// �÷������������������������ͬ
BOOL CRoomManager::Begin(DWORD maxRoomCount)
{
	if (maxRoomCount <= 0)
		return FALSE;

	mMaxRoomCount = maxRoomCount;

	// �����������, �ڿ��Խ��й����ָ��ʸ�������ֵ
	for (DWORD i=0;i<mMaxRoomCount;i++)
	{
		CRoom *RoomClassic = new CRoom();
		CRoom *RoomDouble = new CRoom();
		CRoom *RoomTrible = new CRoom();

		// ���������þ��䶷��������Ϊ i
		if (RoomClassic->Begin(i))
			mRoomVectorClassic.push_back(RoomClassic);
		else
		{
			// ��ʧ�ܵ��������������
			End();
			return FALSE;
		}

		// ���������ö�������������Ϊ i
		if (RoomDouble->Begin(i))
			mRoomVectorDouble.push_back(RoomDouble);
		else
		{
			// ��ʧ�ܵ��������������
			End();
			return FALSE;
		}

		// ���������������Ҷ�������Ϊ i
		if (RoomTrible->Begin(i))
			mRoomVectorTrible.push_back(RoomTrible);
		else
		{
			// ��ʧ�ܵ��������������
			End();
			return FALSE;
		}
	}

	return TRUE;
}

// ����������
BOOL CRoomManager::End(VOID)
{
	CThreadSync Sync;

	for (DWORD i=0;i<mRoomVectorClassic.size();i++)
	{
		CRoom *Room = mRoomVectorClassic[i];
		Room->End();
		delete Room;
	}
	mRoomVectorClassic.clear();

	for (DWORD i=0;i<mRoomVectorDouble.size();i++)
	{
		CRoom *Room = mRoomVectorDouble[i];
		Room->End();
		delete Room;
	}
	mRoomVectorDouble.clear();

	for (DWORD i=0;i<mRoomVectorTrible.size();i++)
	{
		CRoom *Room = mRoomVectorTrible[i];
		Room->End();
		delete Room;
	}
	mRoomVectorTrible.clear();

	return TRUE;
}

// ������ҿ��Խ���ķ���
// ����ѡ��������������еȴ��ķ���
// ������ѡ��շ���
// ���������ַ���������� ����ʧ�� ���ط����ַ NULL
CRoom* CRoomManager::QuickJoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType)
{
	CThreadSync Sync;

	if (!connectedUser)
		return NULL;

	CRoom* Room = NULL;

	// �������䶯̬����
	for(int i = 0; i < mRoomVectorDouble.size(); i++) {
		// ���� i ������ķ���״̬Ϊ RM_WAITING ˵���˷����п�λ ��ҿ��Խ���
		if (mRoomVectorDouble[i]->GetStatus() == ROOM_STATUS_WAITING)
		{
			// ����ҳɹ����뷿��
			if (mRoomVectorDouble[i]->JoinRoom(connectedUser, roomType)) {
				// �������ַ���Ƶ� Room ��
				Room = mRoomVectorDouble[i];
				// ��������ѭ��
				break;
			}
			// ����ҽ��뷿��ʧ�� ��������
			continue;
		}
		else if (mRoomVectorDouble[i]->GetStatus() == ROOM_STATUS_EMPTY) {
			// ��������״̬Ϊ�յķ��� ��¼�·����ַ �ݲ����뷿��
			Room = mRoomVectorDouble[i];
		}
	}

	// ���������������δ������״̬Ϊ�ȴ��ķ���
	// ����пշ��� ˵����Ҵ�ǰû���ҵ��������
	// ���뵱ǰ�洢�� Room �еĿշ���
	// ���û�пշ��� Room Ϊ NULL ���ҷ���ʧ��
	if (Room != NULL && Room->GetStatus() == ROOM_STATUS_EMPTY) {
		Room->JoinRoom(connectedUser, roomType);
	}

	return Room;
}

// ÿ�붼Ҫ��������������Ϸ����
BOOL CRoomManager::UpdateRooms(CGameIocp *iocp)
{
	CThreadSync Sync;

	// �ظ����з���
	for (int i = 0; i < mRoomVectorDouble.size(); i++)
	{
		CRoom *Room = mRoomVectorDouble[i];

		// �������ڽ��ж�սʱ�Ĵ���
		if (Room->GetStatus() == ROOM_STATUS_GAME_IN_PROGRESS) {

			// ˢ�·�����Ϸ����
			// ͬʱ��ȡ���䵱ǰ״̬ �����Ϸ�Ƿ����
			ROOM_STATUS RoomStatus = Room->UpdateGameProcess();

			// ����Ϸ���� ִ�� Room->GameEnd()
			if (RoomStatus == ROOM_STATUS_GAME_ENDING) {
				Room->GameEnd(iocp);
			}
		}
	}

	return TRUE;
}

// ��ͻ���ͬ������״̬�ĺ���
BOOL CRoomManager::SyncRooms(CGameIocp* iocp)
{
	CThreadSync Sync;

	// �������з���
	for (int i = 0; i < mRoomVectorDouble.size(); i++)
	{
		CRoom* Room = mRoomVectorDouble[i];

		// �������ڽ��ж�սʱ�Ĵ���
		if (Room->GetStatus() == ROOM_STATUS_GAME_IN_PROGRESS) {
			ROOM_STATUS RoomStatus = Room->SyncGameProcess();
		}
	}

	return true;
}
