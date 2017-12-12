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
	ptBattleSearchRoomSuccU.ROOM_STATUS = Room->GetStatus(); 
	ptBattleSearchRoomSuccU.CURRENT_USER_COUNT = Room->GetCurrentUserCount();
	ptBattleSearchRoomSuccU.SIDE_IN_GAME = connectedUser->GetSideInGame();

	connectedUser->WritePacket(
		PT_BATTLE_SEARCH_ROOM_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_SEARCH_ROOM_SUCC_U(WriteBuffer, ptBattleSearchRoomSuccU));

	// 房间处于 等待 状态 函数返回
	if (Room->GetStatus() == ROOM_STATUS_WAITING) {
		printf("RM_WAITING");
		return;
	}
	// 房间处于 就绪 状态 通知各玩家开始游戏
	else if (Room->GetStatus() == ROOM_STATUS_READY) {
		Room->GameStart();
	}
}

// Battle 玩家布设武器请求
VOID CGameIocp::onPT_BATTLE_ARRANGE_WEAPON(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_ARRANGE_WEAPON\n");
	READ_PACKET(PT_BATTLE_ARRANGE_WEAPON);

	CBattleField* BattleField = connectedUser->GetEnteredRoom()->GetBattleField();

	// 读取武器类型
	ENUM_WEAPON_TYPE WeaponType = (ENUM_WEAPON_TYPE)Data.WEAPON_TYPE;
	// 解析武器归属
	ENUM_TROOPS TroopsIn = (ENUM_TROOPS)Data.TROOPS_IN;
	// 读取武器坐标
	float PosX = Data.POS_X;
	float PosY = Data.POS_Y;

	// 将武器加入战场 并获取返回的序号
	Weapon Weapon = BattleField->InputTroops(WeaponType, TroopsIn, PosX, PosY);

	// 建立发送对象并置零
	S_PT_BATTLE_ARRANGE_WEAPON_SUCC_M ptBattleArrangeWeaponSuccM;
	memset(&ptBattleArrangeWeaponSuccM, 0, sizeof(S_PT_BATTLE_ARRANGE_WEAPON_SUCC_M));

	// 武器类型 武器归属 武器坐标 未作改变 直接转发
	ptBattleArrangeWeaponSuccM.WEAPON_TYPE = Weapon.GetProperty().WP_TYPE;
	ptBattleArrangeWeaponSuccM.TROOPS_IN = Weapon.GetTroopsIn();
	ptBattleArrangeWeaponSuccM.POS_X = Weapon.GetPosX();
	ptBattleArrangeWeaponSuccM.POS_Y = Weapon.GetPosY();
	// 武器编号需要从加入战场后返回的对象中读取
	ptBattleArrangeWeaponSuccM.WEAPON_TAG = Weapon.GetWeaponTag();
	// 武器速度需要从加入战场后返回的对象中读取
	ptBattleArrangeWeaponSuccM.SPEED = Weapon.GetProperty().SPEED;
	
	// 建立发送数据的缓冲区
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	// 读取玩家所在房间
	CRoom* room = connectedUser->GetEnteredRoom();
	// 向房间内所有玩家广播武器布设消息
	room->WriteAll(PT_BATTLE_ARRANGE_WEAPON_SUCC_M, WriteBuffer, WRITE_PT_BATTLE_ARRANGE_WEAPON_SUCC_M(WriteBuffer, ptBattleArrangeWeaponSuccM));
}