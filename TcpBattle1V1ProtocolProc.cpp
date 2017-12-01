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

// 3.0 版本添加的头文件
#include "BattleFieldParams.h"

using namespace std;

// 1v1 斗场搜索对手
VOID CGameIocp::onPT_BATTLE_1V1_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_1V1_SEARCH_ROOM\n");

	// 测试时对战时默认用户已经登陆 将用户状态设置为 US_CHANNEL_ENTERED
	// US_CHANNEL_ENTERED 用户登陆后的状态
	connectedUser->SetStatus(US_CHANNEL_ENTERED);

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BATTLE_1V1_SEARCH_ROOM_FAIL_U);
	CLog::WriteLog(_T("# Read packet : PT_BATTLE_1V1_SEARCH_ROOM"));

	// 为玩家寻找游戏房间
	CRoom* Room = mRoomManager.QuickJoinRoom(connectedUser, RM_TYPE_1V1);

	// 未找到房间 通知玩家 寻找房间失败
	if (Room == NULL) {
		connectedUser->WritePacket(
			PT_BATTLE_1V1_SEARCH_ROOM_FAIL_U,
			WriteBuffer,
			WRITE_PT_DOUBLE_BATTLE_SEARCH_ROOM_FAIL_U(WriteBuffer, 1));

		return;
	}

	// 将玩家状态设置为 进入房间
	connectedUser->SetStatus(US_ROOM_ENTERED);

	// 通知玩家找到房间
	S_PT_BATTLE_1V1_SEARCH_ROOM_SUCC_U  ptBattle1V1SearchRoomSuccU;
	memset(&ptBattle1V1SearchRoomSuccU, 0, sizeof(ptBattle1V1SearchRoomSuccU));

	ptBattle1V1SearchRoomSuccU.ROOM_ID = (DWORD_PTR)this;
	ptBattle1V1SearchRoomSuccU.CURRENT_USER_COUNT = Room->GetCurrentUserCount();
	ptBattle1V1SearchRoomSuccU.ROOM_STATUS = Room->GetStatus();

	connectedUser->WritePacket(
		PT_BATTLE_1V1_SEARCH_ROOM_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_1V1_SEARCH_ROOM_SUCC_U(WriteBuffer, ptBattle1V1SearchRoomSuccU));

	// 房间处于 等待 状态 函数返回
	if (Room->GetStatus() == RM_WAITING) {
		printf("RM_WAITING");
		return;
	}
	 // 房间处于 就绪 状态 通知各玩家开始游戏
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