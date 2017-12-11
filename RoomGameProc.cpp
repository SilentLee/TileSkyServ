#include "stdafx.h"

#include "Room.h"

//#include "CriticalSection.h"
//#include "MultiThreadSync.h"
//#include "CircularQueue.h"
//#include "NetworkSession.h"
//#include "PacketSession.h"
//#include "Iocp.h"
//#include "Stream.h"
#include "MemoryPool.h"

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_WritePacket.h"

//#include "ErrorCode.h"


//RM_EMPTY = 0,
//RM_WAITING,
//RM_READY,
//RM_GAME_START,
//RM_GAME_IN_PROGRESS,
//RM_GAME_ENDING,
//RM_GAME_ENDED

ROOM_STATUS CRoom::UpdateGameProcess()
{
	CThreadSync Sync;
	 
	if (mBattleField != NULL) {

		// 更新对战进程 读取此时战场状态
		ENUM_BATTLE_STATUS BattleStatus = mBattleField->UpdateBattleFieldSituation();

		// 更新房间状态
		// 对战进行中 房间状态设置为游戏进行中
		if (BattleStatus == BATTLE_STATUS_IN_PROGRESSING) {
			mStatus = RM_GAME_IN_PROGRESS;
			BroadcastBattleSituation();
		}
		// 蓝方取胜 房间状态设置为游戏结束中
		else if (BattleStatus == BATTLE_STATUS_BLUE_WIN) {
			mStatus = RM_GAME_ENDING;
		}
		// 红方取胜 房间状态设置为游戏结束中
		else if (BattleStatus == BATTLE_STATUS_RED_WIN) {
			mStatus = RM_GAME_ENDING;
		}
		// 平局 房间状态设置为游戏结束中
		else if (BattleStatus == BATTLE_STATUS_DRAW) {
			mStatus = RM_GAME_ENDING;
		}
	}

	// 返回当前房间状态
	return mStatus;
}

void CRoom::BroadcastBattleSituation()
{
	CThreadSync Sync;

	// 打包蓝方战场中的现有兵力
	// 获取蓝方兵力动态数组长度
	int SizeOfTroopsInBlueTeam = mBattleField->getCountOfTroopsInBlueTeam();
	// 创建数据缓冲区
	BYTE PacketBufferTroopsInBlueTeam[3000] = {0, };
	// 创建蓝方兵力数据包长度
	int LengthOfPacketBufferTroopsInBlueTeam = 0;
	// 若当前存储蓝方兵力的动态数组长度大于 0
	if (SizeOfTroopsInBlueTeam > 0) {
		// 获取蓝方兵力动态数组首地址
		Weapon* HeaderOfTroopsInBlueTeam = mBattleField->getTroopInBlueTeam(0);
		// 写入数据
		memcpy(PacketBufferTroopsInBlueTeam, HeaderOfTroopsInBlueTeam, sizeof(Weapon)* SizeOfTroopsInBlueTeam);
		// 计算蓝方兵力数据包长度
		LengthOfPacketBufferTroopsInBlueTeam = sizeof(Weapon)* SizeOfTroopsInBlueTeam;
	}
	// 若当前存储蓝方兵力的动态数组长度等于 0
	// 不能访问该数组 会产生访问越界
	// 所以只存储数组长度 即 0
	else {
		// 计算蓝方兵力数据包长度
		LengthOfPacketBufferTroopsInBlueTeam = 0;
	}

	// 打包红方战场中的现有兵力
	// 获取红方兵力动态数组长度
	int SizeOfTroopsInRedTeam = mBattleField->getCountOfTroopsInRedTeam();
	// 创建数据缓冲区
	BYTE PacketBufferTroopsInRedTeam[3000] = {0, };
	// 创建红方兵力数据包长度
	int LengthOfPacketBufferTroopsInRedTeam = 0;
	// 若当前存储红方兵力的动态数组长度大于 0
	if (SizeOfTroopsInRedTeam > 0) {
		// 获取红方兵力动态数组首地址
		Weapon* HeaderOfTroopsInRedTeam = mBattleField->getTroopInRedTeam(0);
		// 写入数据
		memcpy(PacketBufferTroopsInRedTeam, HeaderOfTroopsInRedTeam, sizeof(Weapon)* SizeOfTroopsInRedTeam);
		// 计算红方兵力数据包长度
		LengthOfPacketBufferTroopsInRedTeam = sizeof(Weapon)* SizeOfTroopsInRedTeam;
	}
	// 若当前存储蓝方兵力的动态数组长度等于 0
	// 不能访问该数组 会产生访问越界
	// 所以只存储数组长度 即 0
	else {
		// 计算红方兵力数据包长度
		LengthOfPacketBufferTroopsInRedTeam = 0;
	}

	S_PT_BATTLE_UPDATE_SITUATION_M ptBattleUpdateSituationM;
	memset(&ptBattleUpdateSituationM, 0, sizeof(S_PT_BATTLE_UPDATE_SITUATION_M));
	// 写入当前游戏剩余时间
	ptBattleUpdateSituationM.REMAINING_GAME_TIME = mBattleField->GetRemainGameTime();
	// 写入蓝方兵力数据长度
	ptBattleUpdateSituationM.BLUE_TROOPS_DATA_LENGTH = SizeOfTroopsInBlueTeam * sizeof(Weapon);
	// 写入红方兵力数据长度
	ptBattleUpdateSituationM.RED_TROOPS_DATA_LENGTH = SizeOfTroopsInRedTeam * sizeof(Weapon);
	// 写入蓝方兵力行为数据长度
	ptBattleUpdateSituationM.BLUE_TROOPS_ACTION_DATA_LENGTH = 0;
	// 写入红方兵力行为数据长度
	ptBattleUpdateSituationM.RED_TROOPS_ACTION_DATA_LENGTH = 0;
	// 写入蓝方兵力数据
	memcpy(ptBattleUpdateSituationM.DATA, PacketBufferTroopsInBlueTeam, LengthOfPacketBufferTroopsInBlueTeam);
	// 写入红方兵力数据
	memcpy(ptBattleUpdateSituationM.DATA + LengthOfPacketBufferTroopsInBlueTeam, PacketBufferTroopsInRedTeam, LengthOfPacketBufferTroopsInRedTeam);
	// 此处应写入蓝方兵力行为数据
	//
	// 此处应写入红方兵力行为数据
	//

	// 向房间内玩家广播数据
	WriteAll(PT_BATTLE_UPDATE_SITUATION_M, (BYTE*)&ptBattleUpdateSituationM, sizeof(S_PT_BATTLE_UPDATE_SITUATION_M));


	//// 创建发送数据缓冲区
	//BYTE PacketBuffer[4000] = {0, };
	//// 计算发送数据包长度
	//int LengthOfPacket = sizeof(int) + LengthOfPacketBufferTroopsInBlueTeam + LengthOfPacketBufferTroopsInRedTeam;
	//// 写入当前游戏时间
	//memcpy(PacketBuffer, &RemainGameTime, sizeof(int));
	//// 复制蓝军兵力数据
	//memcpy(PacketBuffer + sizeof(int), PacketBufferTroopsInBlueTeam, LengthOfPacketBufferTroopsInBlueTeam);
	//// 复制红军兵力数据
	//memcpy(PacketBuffer + sizeof(int) + LengthOfPacketBufferTroopsInBlueTeam, PacketBufferTroopsInRedTeam, LengthOfPacketBufferTroopsInRedTeam);

	//printf("BroadcastBattleSituation PacketLength = %d", LengthOfPacket);

	//// 向房间内所有用户发送数据
	//WriteAll(Protocol, PacketBuffer, LengthOfPacket);




	//// 获取战场中红蓝双方兵力存储动态数组的大小
	//int sizeOfTroopsInBlueTeam = mBattleField->getCountOfTroopsInBlueTeam();
	//int sizeOfTroopsInRedTeam = mBattleField->getCountOfTroopsInRedTeam();

	//// 获取战场中红蓝双方兵力存储动态数组的首地址
	//Weapon* HeaderOfTroopsInBlueTeam = mBattleField->getTroopInBlueTeam(0);
	//Weapon* HeaderOfTroopsInRedTeam = mBattleField->getTroopInRedTeam(0);

	//// 创建广播数据缓冲区
	////BYTE PacketBuffer[MAX_BUFFER_LENGTH] = { 0, };
	//BYTE PacketBuffer[3000] = { 0, };
	//// 设置广播协议号
	//int Protocol = PT_BATTLE_UPDATE_SITUATION_M; // 此处协议号需要重新定义?????????????????????????
	//// 计算广播数据包长度
	//int PacketLength = sizeof(int)+sizeof(int)+sizeof(int)+sizeof(Weapon)* sizeOfTroopsInBlueTeam + sizeof(Weapon)* sizeOfTroopsInRedTeam;

	//// 将剩余游戏时间写入缓冲区
	//memcpy(PacketBuffer, &RemainGameTime, sizeof(int));
	//// 将蓝方兵力动态存储数组大小写入缓冲区
	//memcpy(PacketBuffer + sizeof(int), &sizeOfTroopsInBlueTeam, sizeof(int));
	//// 将红方兵力动态存储数组大小写入缓冲区
	//memcpy(PacketBuffer + sizeof(int)+sizeof(int), &sizeOfTroopsInRedTeam, sizeof(int));
	//// 将蓝方兵力动态存储数组写入缓冲区
	//memcpy(PacketBuffer + sizeof(int)+sizeof(int)+sizeof(int), HeaderOfTroopsInBlueTeam, sizeof(Weapon)* sizeOfTroopsInBlueTeam);
	//// 将红方兵力动态存储数组写入缓冲区
	//memcpy(PacketBuffer + sizeof(int)+sizeof(int)+sizeof(int)+sizeof(Weapon)* sizeOfTroopsInBlueTeam, HeaderOfTroopsInRedTeam, sizeof(Weapon)* sizeOfTroopsInRedTeam);

	//printf("BroadcastBattleSituation PacketLength = %d", PacketLength);

	//// 向房间内所有用户发送广播数据
	//WriteAll(Protocol, PacketBuffer, PacketLength);
}