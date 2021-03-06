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

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

#include "ErrorCode.h"
//#include "GameCtrlParams.h"

extern S_DOUBLE_BATTLE_ROOM_USER GetDoubleBattleRoomUserPacket 
	(S_USER_ACCOUNT_INFO* userAccountInfo, S_USER_EQUIPMENT_INFO* userEquipmentInfo);

CRoom::CRoom(void)
{
	mType = RM_NO_TYPE;
	mStatus = ROOM_STATUS_EMPTY;
	mIndex = 0;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;
}

CRoom::~CRoom(void)
{
}

// 在屏幕上打印传输协议
void CRoom::printProtocol(int protocol)
{	
	switch (protocol)
	{
	case PT_DOUBLE_BATTLE_START_GAME_M:
		printf("PT_DOUBLE_BATTLE_START_GAME_M\n");
		break;
	}
}

// CRoom类的开始函数
// 通过参数转到房间索引
// 在 RoomManager 类中, 调用 Begin( i ) 函数时, 通过 for 循环将第 i 个生成的 Room 的编号定为 i
bool CRoom::Begin(DWORD index)
{
	CThreadSync Sync;

	mType = RM_NO_TYPE;
	mStatus = ROOM_STATUS_EMPTY;
	mIndex = index;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;

	return true;
}

//结束对象的函数
bool CRoom::End(void)
{
	CThreadSync Sync;

	mType = RM_NO_TYPE;
	mStatus = ROOM_STATUS_EMPTY;
	mIndex = 0;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;

	return true;
}

// 用户进入房间时进行处理的函数
bool CRoom::JoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType)
{
	CThreadSync Sync;

	// 若房间类型已经确定且与当前申请游戏的房间类型不符 函数返回
	if (mType != RM_NO_TYPE && mType != roomType)
		return false;

	mType = roomType;

	// 若玩家指针无效 返回
	if (!connectedUser)
		return false;

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};

	int capacityOfRoom, capacityOfTeam;

	// 若根据当前房间类型 设置红蓝两队人数上限
	if (mType == RM_TYPE_1V1) {
		capacityOfRoom = NUM_OF_PLAYERS_FOR_1V1;
		capacityOfTeam = NUM_OF_PLAYERS_FOR_1V1 / 2;
	}
	else if (mType == RM_TYPE_2V2) {
		capacityOfRoom = NUM_OF_PLAYERS_FOR_2V2;
		capacityOfTeam = NUM_OF_PLAYERS_FOR_2V2 / 2;
	}

	// 若蓝队人数未达上线 优先将玩家分配到蓝队
	if (mUsersInBlueTeam.size() < capacityOfTeam) {
		mUsersInBlueTeam.push_back(connectedUser);
		connectedUser->SetEnteredRoom(this);
		connectedUser->SetSideInGame(SIDE_BLUE);
	}
	// 若蓝队人数已达上限 将玩家分配到红队
	else if (mUsersInRedTeam.size() < capacityOfTeam) {
		mUsersInRedTeam.push_back(connectedUser);
		connectedUser->SetEnteredRoom(this);
		connectedUser->SetSideInGame(SIDE_RED);
	}

	// ????????????????????????????????????????????????????
	// 此处需要在 CConnectedUser 中配置玩家的队友和对手 代码待完成
	

	// 若当前玩家加入房间后 房间内人数满足游戏开始条件 将房间更改为 RM_READY
	if (mUsersInBlueTeam.size() + mUsersInRedTeam.size() == capacityOfRoom) {
		mStatus = ROOM_STATUS_READY;
	}
	// 若当前玩家加入房间后 房间内人数仍然不满足游戏开始条件 将房间状态设置为 RM_WAITINIG
	else
	{
		mStatus = ROOM_STATUS_WAITING;
	}

	return true;
}

void CRoom::SetUserStatusAll(ENUM_USER_STATUS status)
{
	CThreadSync Sync;
	
	list<CConnectedUser*>::iterator iter;
	
	for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); iter++) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->SetStatus(status);
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); iter++) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->SetStatus(status);
	}
}

 // 用户退出房间时调用的函数
 // 确认是否结束登录并推出房间的参数isDisconnected, 一个人退出房间时结束游戏的参数iocp
 // 推出用户对象connectedUser
bool CRoom::LeaveRoom(bool isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser)
{
	CThreadSync Sync;

	//建立发送数据包的临时缓冲区
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
	S_PT_DOUBLE_BATTLE_USER_LEAVE_M ptDoubleBattleUserLeaveM;

	// 在用户对象出错的情况下, 失败
	if (!connectedUser)
		return false;

	list<CConnectedUser*>::iterator iter;

	// 若玩家当前在游戏房间内
	// 在红蓝两队列表中删除 CConnectedUser 指针
	// 通知房间内其他玩家 有人退出当前房间
	if(connectedUser->GetStatus() <= US_GAME_IN_PROGRESS) {

		for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) 
		{
			CConnectedUser* ConnectedUser = (CConnectedUser*)(*iter);
			if(ConnectedUser == connectedUser) 
			{
				mUsersInBlueTeam.erase(iter);
				ptDoubleBattleUserLeaveM.USER_ID = ConnectedUser->GetUserAccountInfo()->USER_ID;
				WriteAll(PT_DOUBLE_BATTLE_USER_LEAVE_M, 
								WriteBuffer, 
								WRITE_PT_DOUBLE_BATTLE_USER_LEAVE_M(WriteBuffer, ptDoubleBattleUserLeaveM));
				return true;
			}
		}

		for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter)
		{
			CConnectedUser* ConnectedUser = (CConnectedUser*)(*iter);
			if (ConnectedUser == connectedUser)
			{
				mUsersInRedTeam.erase(iter);
				ptDoubleBattleUserLeaveM.USER_ID = ConnectedUser->GetUserAccountInfo()->USER_ID;
				WriteAll(PT_DOUBLE_BATTLE_USER_LEAVE_M,
					WriteBuffer,
					WRITE_PT_DOUBLE_BATTLE_USER_LEAVE_M(WriteBuffer, ptDoubleBattleUserLeaveM));
				return true;
			}
		}

		// 在用户列表中没有检索到游戏玩家 返回 false
		return false;

	} 
	// 若此时玩家状态为进行游戏 此时不能简单退出房间
	// 应由AI接管游戏 或 等待游戏结束再退出房间
	// 代码待完善???????????????????????????????????????????????????
	//else if(connectedUser->GetStatus() <= US_GAME_IN_PROGRESS){

	//	connectedUser->SetStatus(connectedUser->GetStatus() + 4); // 游戏开始后用户断开连接, 由 AI 接管
	//	return true;
	//}

	return false;
}

// 向房间内的所有用户传送数据包
bool CRoom::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (protocol <= 0 || !packet)
		return false;
	
	// 在屏幕上打印发送数据包的协议信息
	printProtocol(protocol);

	// 遍历当前红蓝两队用户列表 群发数据
	list<CConnectedUser*>::iterator iter;
	
	for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->WritePacket(protocol, packet, packetLength);

		// 每完成1次数据发送后 休眠一定时间 防止丢包
		Sleep(10);
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->WritePacket(protocol, packet, packetLength);

		// 每完成1次数据发送后 休眠一定时间 防止丢包
		Sleep(10);
	}

	return true;
}

// 比赛开始时调用的函数
bool CRoom::GameStart(void)
{
	CThreadSync Sync;

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};

	// 设置房间内所有玩家状态为 US_GAME_START
	SetUserStatusAll(US_GAME_START);
	// 设置房间状态为 RM_GAME_START
	SetStatus(ROOM_STATUS_GAME_START);
	// 设置房间游戏开始标志位为 true
	mIsGameStarted = true;

	// 生成战场
	mBattleField = new CBattleField();
	mBattleField->Begin();

	// 遍历玩家列表 对玩家进行游戏开始前的初始化
	list<CConnectedUser*>::iterator iter;
	for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if(!connectedUser->InitializeForGameStart()) {
			return false;
		}
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if (!connectedUser->InitializeForGameStart()) {
			return false;
		}
	}

	// 通知各玩家游戏开始 在游戏开始消息成功发送之前 不能将房间状态 mStatus 更改为 RM_GAME_IN_PROGRESS
	S_PT_BATTLE_START_GAME_M ptBattleStartGameM;
	memset(&ptBattleStartGameM, 0, sizeof(S_PT_BATTLE_START_GAME_M));
	//time(&ptBattleStartGameM.START_TIME);
	ptBattleStartGameM.ROOM_STATUS = GetStatus();
	//ptBattleStartGameM.ROOM_TYPE = GetType();

	WriteAll(PT_BATTLE_START_GAME_M, WriteBuffer, WRITE_PT_BATTLE_START_GAME_M(WriteBuffer, ptBattleStartGameM));

	//// 游戏开始消息发送成功后 将房间状态更改为 RM_GAME_IN_PROGRESS
	SetStatus(ROOM_STATUS_GAME_IN_PROGRESS);

	// 将战场设置为游戏开始
	mBattleField->BattleStart();

	return true;
}

// 游戏结束时, 求得两队HP总和, 多的一方获胜
// 可以在这里将游戏结束后, 用户信息的改变与数据库进行同步
// 向房间内所有用户传送游戏结果数据包
// 重新设定用户状态, 改为等待, 初始化游戏相关变量
bool CRoom::GameEnd(CGameIocp *iocp)
{
	CThreadSync Sync;

	mStatus = ROOM_STATUS_GAME_ENDED;
	mIsGameStarted = false;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	S_PT_DOUBLE_BATTLE_END_M ptDoubleBattleEndM;
	memset(&ptDoubleBattleEndM, 0, sizeof(ptDoubleBattleEndM));

	//????????????????????????????????????
	// 此处应该读取游戏对战结果信息 分发给每个游戏玩家

	// 遍历玩家列表 设置玩家游戏结束
	list<CConnectedUser*>::iterator iter;
	for (iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if (!connectedUser->GameEnd()) {
			return false;
		}
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if (!connectedUser->GameEnd()) {
			return false;
		}
	}

	mType = RM_NO_TYPE;
	mStatus = ROOM_STATUS_EMPTY;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;

	// 销毁战场
	delete mBattleField;
	mBattleField = NULL;

	return true;
}