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

#include "BATTLE_1V1_Protocol.h"
#include "BATTLE_1V1_PT_Structure.h"
#include "BATTLE_1V1_PT_ReadPacket.h"
#include "BATTLE_1V1_PT_WritePacket.h"

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

// 1v1 ������������
VOID CGameIocp::onPT_BATTLE_1V1_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_1V1_SEARCH_ROOM\n");

	// ����ʱ��սʱĬ���û��Ѿ���½ ���û�״̬����Ϊ US_CHANNEL_ENTERED
	// US_CHANNEL_ENTERED �û���½���״̬
	connectedUser->SetStatus(US_CHANNEL_ENTERED);

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BATTLE_1V1_SEARCH_ROOM_FAIL_U);
	CLog::WriteLog(_T("# Read packet : PT_BATTLE_1V1_SEARCH_ROOM"));

	// Ϊ���Ѱ����Ϸ����
	CRoom* Room = mRoomManager.QuickJoinRoom(connectedUser, RM_TYPE_1V1);

	// δ�ҵ����� ֪ͨ��� Ѱ�ҷ���ʧ��
	if (Room == NULL) {
		connectedUser->WritePacket(
			PT_BATTLE_1V1_SEARCH_ROOM_FAIL_U,
			WriteBuffer,
			WRITE_PT_DOUBLE_BATTLE_SEARCH_ROOM_FAIL_U(WriteBuffer, 1));

		return;
	}

	// �����״̬����Ϊ ���뷿��
	connectedUser->SetStatus(US_ROOM_ENTERED);

	// ֪ͨ����ҵ�����
	S_PT_BATTLE_1V1_SEARCH_ROOM_SUCC_U  ptBattle1V1SearchRoomSuccU;
	memset(&ptBattle1V1SearchRoomSuccU, 0, sizeof(ptBattle1V1SearchRoomSuccU));

	ptBattle1V1SearchRoomSuccU.ROOM_ID = (DWORD_PTR)this;
	ptBattle1V1SearchRoomSuccU.CURRENT_USER_COUNT = Room->GetCurrentUserCount();
	ptBattle1V1SearchRoomSuccU.ROOM_STATUS = Room->GetStatus();

	connectedUser->WritePacket(
		PT_BATTLE_1V1_SEARCH_ROOM_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_1V1_SEARCH_ROOM_SUCC_U(WriteBuffer, ptBattle1V1SearchRoomSuccU));

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

VOID CGameIocp::onPT_BATTLE_1V1_ARRANGE_CARD(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_1V1_ARRANGE_CARD\n");
	READ_PACKET(PT_BATTLE_1V1_ARRANGE_CARD);

	S_PT_BATTLE_1V1_ARRANGE_CARD_SUCC_U  ptBattle1V1ArrangeCardSuccU;
	memset(&ptBattle1V1ArrangeCardSuccU, 0, sizeof(ptBattle1V1ArrangeCardSuccU));

	ptBattle1V1ArrangeCardSuccU.CARD_TYPE = Data.CARD_TYPE;

	ptBattle1V1ArrangeCardSuccU.COORDINATE_X = Data.COORDINATE_X;
	ptBattle1V1ArrangeCardSuccU.COORDINATE_Y = (BATTLE_FIELD_HEIGHT_IN_SQUARE - 1) - Data.COORDINATE_Y;
	ptBattle1V1ArrangeCardSuccU.POS_X = Data.POS_X;
	ptBattle1V1ArrangeCardSuccU.POS_Y = 2060 - Data.POS_Y;
	ptBattle1V1ArrangeCardSuccU.SPEED = Data.SPEED;

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	connectedUser->WritePacket(PT_BATTLE_1V1_ARRANGE_CARD_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_1V1_ARRANGE_CARD_SUCC_U(WriteBuffer, ptBattle1V1ArrangeCardSuccU));

	printf("Send PT_BATTLE_1V1_ARRANGE_CARD_SUCC_U\n");
}