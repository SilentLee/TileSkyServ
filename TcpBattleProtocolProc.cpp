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
#include "DoubleBattleProtocol.h"
#include "DOUBLE_BATTLE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_ReadPacket.h"
#include "DOUBLE_BATTLE_PT_WritePacket.h"

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"
#include "GameCtrlParams.h"
#include "Random.h"
#include "ErrCodes.h"

// 3.0 �汾��ӵ�ͷ�ļ�
#include "BattleFieldParams.h"

using namespace std;

// Battle ��������
VOID CGameIocp::onPT_BATTLE_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_SEARCH_ROOM\n");
	READ_PACKET(PT_BATTLE_SEARCH_ROOM);

	// ����ʱ��սʱĬ���û��Ѿ���½ ���û�״̬����Ϊ US_CHANNEL_ENTERED
	// US_CHANNEL_ENTERED �û���½���״̬
	connectedUser->SetStatus(US_CHANNEL_ENTERED);

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BATTLE_SEARCH_ROOM_FAIL_U);
	CLog::WriteLog(_T("# Read packet : PT_BATTLE_SEARCH_ROOM"));

	ROOM_TYPE roomType = (ROOM_TYPE)Data.ROOM_TYPE;

	// Ϊ���Ѱ����Ϸ����
	CRoom* Room = mRoomManager.QuickJoinRoom(connectedUser, roomType);

	// δ�ҵ����� ֪ͨ��� Ѱ�ҷ���ʧ��
	if (Room == NULL) {
		connectedUser->WritePacket(
			PT_BATTLE_SEARCH_ROOM_FAIL_U,
			WriteBuffer,
			WRITE_PT_DOUBLE_BATTLE_SEARCH_ROOM_FAIL_U(WriteBuffer, 1));

		return;
	}

	// �����״̬����Ϊ ���뷿��
	connectedUser->SetStatus(US_ROOM_ENTERED);

	// ֪ͨ����ҵ�����
	S_PT_BATTLE_SEARCH_ROOM_SUCC_U  ptBattleSearchRoomSuccU;
	memset(&ptBattleSearchRoomSuccU, 0, sizeof(ptBattleSearchRoomSuccU));

	// ROOM_ID ��ǰ����ָ��ֵ
	// CURRENT_USER_COUNT ��ǰ������������� �������� + �������
	// ROOM_STATUS ��ǰ����״̬
	// ROOM_STATUS = RM_READY �ﵽ��ʼ��Ϸ������Ҫ�� 
	// ROOM_STATUS = RM_WAITING δ�ﵽ��ʼ��Ϸ������Ҫ�� 
	ptBattleSearchRoomSuccU.ROOM_ID = (DWORD_PTR)this;
	ptBattleSearchRoomSuccU.ROOM_TYPE = Room->GetType();
	ptBattleSearchRoomSuccU.CURRENT_USER_COUNT = Room->GetCurrentUserCount();
	ptBattleSearchRoomSuccU.ROOM_STATUS = Room->GetStatus(); 

	connectedUser->WritePacket(
		PT_BATTLE_SEARCH_ROOM_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_SEARCH_ROOM_SUCC_U(WriteBuffer, ptBattleSearchRoomSuccU));

	// ���䴦�� �ȴ� ״̬ ��������
	if (Room->GetStatus() == RM_WAITING) {
		printf("RM_WAITING");
		return;
	}
	// ���䴦�� ���� ״̬ ֪ͨ����ҿ�ʼ��Ϸ
	else if (Room->GetStatus() == RM_READY) {
		Room->GameStart();
	}
}

// Battle ��Ҳ�����������
VOID CGameIocp::onPT_BATTLE_ARRANGE_WEAPON(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_ARRANGE_WEAPON\n");
	READ_PACKET(PT_BATTLE_ARRANGE_WEAPON);

	CBattleField* BattleField = connectedUser->GetEnteredRoom()->GetBattleField();

	// ������ӱ���������Ϸ����Ҷ�Ӧ��һ��
	if (connectedUser->GetSideInGame() == SIDE_BLUE) {
		BattleField->InputTroopsBlue(Data.WEAPON_TYPE, Data.POS_X, Data.POS_Y);
	}
	else if (connectedUser->GetSideInGame() == SIDE_RED) {
		BattleField->InputTroopsRed(Data.WEAPON_TYPE, Data.POS_X, Data.POS_Y);
	}

	//S_PT_BATTLE_ARRANGE_CARD_SUCC_U  ptBattleArrangeCardSuccU;
	//memset(&ptBattleArrangeCardSuccU, 0, sizeof(ptBattleArrangeCardSuccU));

	//ptBattleArrangeCardSuccU.CARD_TYPE = Data.CARD_TYPE;

	//ptBattleArrangeCardSuccU.COORDINATE_X = Data.COORDINATE_X;
	//ptBattleArrangeCardSuccU.COORDINATE_Y = (BATTLE_FIELD_HEIGHT_IN_SQUARE - 1) - Data.COORDINATE_Y;
	//ptBattleArrangeCardSuccU.POS_X = Data.POS_X;
	//ptBattleArrangeCardSuccU.POS_Y = 2060 - Data.POS_Y;
	//ptBattleArrangeCardSuccU.SPEED = Data.SPEED;

	//BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	//connectedUser->WritePacket(PT_BATTLE_ARRANGE_CARD_SUCC_U,
	//	WriteBuffer,
	//	WRITE_PT_BATTLE_ARRANGE_CARD_SUCC_U(WriteBuffer, ptBattleArrangeCardSuccU));

	//printf("Send PT_BATTLE_ARRANGE_CARD_SUCC_U\n");
}