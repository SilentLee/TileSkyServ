#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "Stream.h"
#include "IniFile.h"

#include "ErrorCode.h"
#include "UserProtocol.h"
#include "User_PT_Structure.h"
#include "User_PT_ReadPacket.h"
#include "User_PT_WritePacket.h"

#include "PurchaseProtocol.h"
#include "PURCHASE_PT_Structure.h"
#include "PURCHASE_PT_ReadPacket.h"
#include "PURCHASE_PT_WritePacket.h"

#include "DoubleBattleProtocol.h"
#include "DOUBLE_BATTLE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_ReadPacket.h"
#include "DOUBLE_BATTLE_PT_WritePacket.h"

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

// DWORD dataLength Ϊ�������ݵĳ���
VOID CGameIocp::OnIoRead(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	DWORD	Protocol						= 0;
	BYTE	Packet[MAX_BUFFER_LENGTH]		= {0,};
	DWORD	PacketLength					= 0;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (ConnectedUser->ReadPacketForIocp(dataLength))
	{
		while (ConnectedUser->GetPacket(Protocol, Packet, PacketLength))
		{
			switch(Protocol)
			{
			case PT_BATTLE_SEARCH_ROOM:
				onPT_BATTLE_SEARCH_ROOM(ConnectedUser, Packet);
				break;

			case PT_BATTLE_ARRANGE_WEAPON:
				onPT_BATTLE_ARRANGE_WEAPON(ConnectedUser, Packet);
				break;
			//// �û��˻�ע�� ��¼ ��Ϣ�������ݰ�����
			//// 1. �ο�ע��
			//case PT_VISITOR_REGIST:
			//	onPT_VISITOR_REGIST(ConnectedUser, Packet);
			//	break;
			//// 2. �û���ע��
			//case PT_USER_NAME_REGIST:
			//	onPT_USER_NAME_REGIST(ConnectedUser, Packet);
			//	break;
			//// 3. ���˻�ע��
			//case PT_BIND_ACCOUNT_REGIST:
			//	onPT_BIND_ACCOUNT_REGIST(ConnectedUser, Packet);
			//	break;
			//// 4. �ֻ�����ע��
			//case PT_CELLPHONE_NO_REGIST:
			//	onPT_CELLPHONE_NO_REGIST(ConnectedUser, Packet);
			//	break;
			//// 5. �û�����¼
			//case PT_USER_NAME_LOGIN:
			//	onPT_USER_NAME_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 6. ���˻���¼
			//case PT_BIND_ACCOUNT_LOGIN:
			//	onPT_BIND_ACCOUNT_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 7. �ֻ������¼
			//case PT_CELLPHONE_NO_LOGIN:
			//	onPT_CELLPHONE_NO_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 8. �ο�Ĭ�ϵ�¼
			//case PT_VISITOR_DEFAULT_LOGIN:
			//	onPT_VISITOR_DEFAULT_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 12. ����ǳ�
			//case PT_ADD_NICKNAME:
			//	onPT_ADD_NICKNAME(ConnectedUser, Packet);
			//	break;
			//// 13. ��Ӱ��˻�
			//case PT_ADD_BIND_ACCOUNT:
			//	onPT_ADD_BIND_ACCOUNT(ConnectedUser, Packet);
			//	break;
			//// 14. ����ֻ�����
			//case PT_ADD_CELLPHONE_NO:
			//	onPT_ADD_CELLPHONE_NO(ConnectedUser, Packet);
			//	break;
			//// 15. ��������
			//case PT_UPDATE_PASSWORD:
			//	onPT_UPDATE_PASSWORD(ConnectedUser, Packet);
			//	break;
			//// 16. �����ǳ�
			//case PT_UPDATE_NICKNAME:
			//	onPT_UPDATE_NICKNAME(ConnectedUser, Packet);
			//	break;
			//// 17. �����ֻ�����
			//case PT_UPDATE_CELLPHONE_NO:
			//	onPT_UPDATE_CELLPHONE_NO(ConnectedUser, Packet);
			//	break;
			//// 18. �����û�ս��
			//case PT_UPDATE_RECORD:
			//	onPT_UPDATE_RECORD(ConnectedUser, Packet);
			//	break;
			//	
			//// �û������������ݰ�����
			//// 1. ������
			//case PT_BUY_COINS:
			//	onPT_BUY_COINS(ConnectedUser, Packet);
			//	break;
			//// 2. ������ʯ
			//case PT_BUY_DIAMONDS:
			//	onPT_BUY_DIAMONDS(ConnectedUser, Packet);
			//	break;
			//// 3. ������
			//case PT_BUY_CARDS:
			//	onPT_BUY_CARDS(ConnectedUser, Packet);
			//	break;

			//case PT_ACQUIRE_CHEST:
			//	onPT_ACQUIRE_CHEST(ConnectedUser, Packet);
			//	break;

			//// 5. �򿪲�����
			//case PT_OPEN_SUPPLY_CHEST:
			//	onPT_OPEN_SUPPLY_CHEST(ConnectedUser, Packet);
			//	break;

			//case PT_LOAD_CARDS_ON_SALE:
			//	onPT_LOAD_CARDS_ON_SALE(ConnectedUser, Packet);
			//	break;

			//case PT_UPGRADE_CARD:
			//	onPT_UPGRADE_CARD(ConnectedUser, Packet);
			//	break;

			//// Ѱ�Ҷ�ս����
			//case PT_DOUBLE_BATTLE_SEARCH_ROOM:
			//	onPT_DOUBLE_BATTLE_SEARCH_ROOM(ConnectedUser, Packet);
			//	break;

			//// ���տͻ��˷��͵ķɻ�������Ϣ
			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_STEALTH_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_STEALTH_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_STEALTH_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_STEALTH_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_STEALTH_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_STEALTH_PLANE(ConnectedUser, Packet);
			//	break;

			//// ���տͻ��˷���������������Ϣ
			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_ANTI_STEALTH_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_ANTI_STEALTH_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_ANTI_STEALTH_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_ANTI_STEALTH_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_ANTI_STEALTH_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_ANTI_STEALTH_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_MISSILE:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_MISSILE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_MISSILE:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_MISSILE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_MISSILE:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_MISSILE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_RADAR(ConnectedUser, Packet);
			//	break;
			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_CANNONBALL:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_CANNONBALL(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_CANNONBALL:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_CANNONBALL(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_CANNONBALL:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_CANNONBALL(ConnectedUser, Packet);
			//	break;
			}
		}
	}

	// ��ȡ���ݺ�, �ָ��ȴ������ȡ���ݵ�״̬
	if (!ConnectedUser->InitializeReadForIocp())
		ConnectedUser->End();
	// TODO : ǿ�ƽ�������
	// Endһ������֮��
	// ConnectedUser->End();
}

// ���º���: OnIoConnected, OnIoDisconnected, OnIoRead, OnIoWrote
// �ɸ��� CIocp �е� WorkerThreadCallback ����
VOID CGameIocp::OnIoConnected(VOID *object)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	TCHAR	RemoteAddress[32]	= {0,};

	// ��õ�¼��ʵ�����ĵ�ַ
	USHORT	RemotePort			= 0;

	// ���Accept֮���¼��ʵ������ַ
	ConnectedUser->GetRemoteAddressAfterAccept(RemoteAddress, RemotePort);

	// ������ȡ�ĵ�ַ�趨��ConnectedUser
	ConnectedUser->SetRealInformation(RemoteAddress, RemotePort);

	// ���ͻ��˵� Socket ��������ɶ˿ڰ�
	if (!CIocp::RegisterSocketToIocp(ConnectedUser->GetSocket(), (ULONG_PTR) ConnectedUser))
	{
		CLog::WriteLog(_T("! OnIoConnected : CIocp::RegisterSocketToIocp"));

		End();

		return;
	}
	// ������ת��Ϊ��¼״̬
	ConnectedUser->SetIsConnected(TRUE);

	// ���л�����Iocp��ȡ
	if (!ConnectedUser->InitializeReadForIocp())
	{
		if (!ConnectedUser->Reload(mListenSession.GetSocket()))
		{
			CLog::WriteLog(_T("! OnIoConnected : ConnectedUser->Reload"));

			End();

			return;
		}
	}

	// ����ConnectedUserManager�ĵ�ǰ��������
	mConnectedUserManager.IncreaseConnectedUserCount();

	CLog::WriteLog(_T("# New client connected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

VOID CGameIocp::OnIoDisconnected(VOID *object)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	//ConnectedUser->db_UpdateUserAccountInfo(mMySqlSessionPool);
	while(!ConnectedUser->db_UpdateUserAccountInfo(mMySqlSessionPool)) {};

	// �˳��û����ڷ����ڵ����
	if (ConnectedUser->GetStatus() >= US_ROOM_ENTERING)
	{
		// ֪ͨ�����û���ʹ���˳�����
		CRoom *Room = ConnectedUser->GetEnteredRoom();

		if (Room)
			Room->LeaveRoom(TRUE, this, ConnectedUser);

		// ����ǰ�����Ѿ�û�����
		// ������ǰ�����ڵ���Ϸ
		if (!Room->GetCurrentUserCount()) {
			Room->GameEnd(this);
		}
	}

	// ����ǰ�û����ӱ� AI �ӹ�, ���û��Ͽ�����ʱ��������������������, ��ȱ�����Ϸ�������ٶ��������ӽ�������
	// �˴����������?????????????????????????????????????????????????????????????????????????????????
	//if(ConnectedUser->GetStatus() >= AI_GAME_START && ConnectedUser->GetStatus() <= AI_GAME_ENDING) {
	//	return;
	//}

	// �û��Ͽ����Ӻ�, ���� ConnectedUser ��������Ϊ Accept ״̬, �ȴ���һ���û�����
	if (!ConnectedUser->Reload(mListenSession.GetSocket()))
	{
		CLog::WriteLog(_T("! OnIoDisconnected : ConnectedUser->Reload"));
		// �� CConnectedUser ��������ʧ�� ������ǰ iocp ����
		End();

		return;
	}

	// ��Ϊ�Ͽ�һ������, ConnectedUserManager �е�ǰ�����û�����ֵ�� 1
	mConnectedUserManager.DecreaseConnectedUserCount();

	CLog::WriteLog(_T("# Client disconnected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

// OnIoWrite ֻ�轫д���֮������ݵ����������ݶ��м���
VOID CGameIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;
	// д�����ʱ����еĴ���
	ConnectedUser->WriteComplete(); // �� WriteComplete ������, ��������ɵ����ݵ����������ݶ���

#ifdef _FULL_DEBUG
	CLog::WriteLog(_T("# Client data wrote : 0x%x(0x%x)(%d)\n"), ConnectedUser, ConnectedUser->GetSocket(), dataLength);
#endif
}

























