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

	CRoom* RoomEmpty = NULL;

	// �������䶯̬����
	for(int i = 0; i < mRoomVectorDouble.size(); i++) {
		CRoom* Room = mRoomVectorDouble[i];

		// ���� i ������ķ���״̬Ϊ RM_WAITING ˵���˷����п�λ ��ҿ��Խ���
		if(Room->GetStatus() == RM_WAITING)
		{
			// ��ҽ��뷿�� ���� Room ��ַ
			if (Room->JoinRoom(connectedUser, roomType))
				return Room;
			// �����뷿��ʧ�� ��������
			continue;
		} else if(Room->GetStatus() == RM_EMPTY) {
			// ��������״̬Ϊ�յķ��� ��¼�·����ַ �ݲ����뷿��
			RoomEmpty = Room;
		}
	}

	// ���������������δ������״̬Ϊ�ȴ��ķ���
	if(RoomEmpty) {
		// ����ǰ��ҷ���շ���ȴ���һ����ҽ��� ͬʱ�� JoniUserDouble �����н� RoomEmpty ״̬����Ϊ RM_WAITING
		RoomEmpty->JoinRoom(connectedUser, roomType);
	}

	return RoomEmpty;
}

// ÿ�붼Ҫ��������������Ϸ����
BOOL CRoomManager::UpdateRooms(CGameIocp *iocp)
{
	CThreadSync Sync;

	// �ظ����з���
	for (DWORD i = 0; i < mRoomVectorDouble.size(); i++)
	{
		CRoom *Room = mRoomVectorDouble[i];
		CBattleField* BattleField = Room->getBattleField();

		// ս����Ϊ�� �򷿼䴦�ڶ�ս��
		if (BattleField != NULL) {
			// ˢ��ս��̬�� ��ȡ��ǰ��ս���
			BATTLE_STATUS BattleStatus = BattleField->UpdateBattleFieldSituation();
			// ��δ����ʤ�� �򷿼���������ҹ㲥ս��̬�Ƹ�������
			if (BattleStatus == BATTLE_STATUS_IN_PROGRESSING || BattleStatus == BATTLE_STATUS_IN_PREPARING) {
				Room->GameEnd(iocp); // �˴�����
			}
			// ������ʤ�� ��Ϸ����
			else {

			}
		}
	}

	return TRUE;
}
