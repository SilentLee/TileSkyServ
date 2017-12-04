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

// 3.0 版本添加的头文件
#include "BattleFieldParams.h"

using namespace std;

// Battle 搜索对手
VOID CGameIocp::onPT_BATTLE_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_SEARCH_ROOM\n");
	READ_PACKET(PT_BATTLE_SEARCH_ROOM);

	// 测试时对战时默认用户已经登陆 将用户状态设置为 US_CHANNEL_ENTERED
	// US_CHANNEL_ENTERED 用户登陆后的状态
	connectedUser->SetStatus(US_CHANNEL_ENTERED);

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BATTLE_SEARCH_ROOM_FAIL_U);
	CLog::WriteLog(_T("# Read packet : PT_BATTLE_SEARCH_ROOM"));

	ROOM_TYPE roomType = (ROOM_TYPE)Data.ROOM_TYPE;

	// 为玩家寻找游戏房间
	CRoom* Room = mRoomManager.QuickJoinRoom(connectedUser, roomType);

	// 未找到房间 通知玩家 寻找房间失败
	if (Room == NULL) {
		connectedUser->WritePacket(
			PT_BATTLE_SEARCH_ROOM_FAIL_U,
			WriteBuffer,
			WRITE_PT_DOUBLE_BATTLE_SEARCH_ROOM_FAIL_U(WriteBuffer, 1));

		return;
	}

	// 将玩家状态设置为 进入房间
	connectedUser->SetStatus(US_ROOM_ENTERED);

	// 通知玩家找到房间
	S_PT_BATTLE_SEARCH_ROOM_SUCC_U  ptBattleSearchRoomSuccU;
	memset(&ptBattleSearchRoomSuccU, 0, sizeof(ptBattleSearchRoomSuccU));

	// ROOM_ID 当前房间指针值
	// CURRENT_USER_COUNT 当前房间内玩家人数 蓝队人数 + 红队人数
	// ROOM_STATUS 当前房间状态
	// ROOM_STATUS = RM_READY 达到开始游戏的人数要求 
	// ROOM_STATUS = RM_WAITING 未达到开始游戏的人数要求 
	ptBattleSearchRoomSuccU.ROOM_ID = (DWORD_PTR)this;
	ptBattleSearchRoomSuccU.ROOM_TYPE = Room->GetType();
	ptBattleSearchRoomSuccU.CURRENT_USER_COUNT = Room->GetCurrentUserCount();
	ptBattleSearchRoomSuccU.ROOM_STATUS = Room->GetStatus(); 

	connectedUser->WritePacket(
		PT_BATTLE_SEARCH_ROOM_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_SEARCH_ROOM_SUCC_U(WriteBuffer, ptBattleSearchRoomSuccU));

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

// Battle 玩家布设武器请求
VOID CGameIocp::onPT_BATTLE_ARRANGE_WEAPON(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_ARRANGE_WEAPON\n");
	READ_PACKET(PT_BATTLE_ARRANGE_WEAPON);

	CBattleField* BattleField = connectedUser->GetEnteredRoom()->GetBattleField();

	// 将新添加兵力加入游戏中玩家对应的一方
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