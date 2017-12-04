#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Room.h"
#include "ConnectedUser.h"
#include "GameCtrlParams.h"

CConnectedUser::CConnectedUser(void)
{
	ZeroMemory(&mUserAccountInfo, sizeof(S_USER_ACCOUNT_INFO));
	ZeroMemory(&mUserEquipmentInfo, sizeof(S_USER_EQUIPMENT_INFO));
	ZeroMemory(&mUserChestInfo, sizeof(S_USER_CHEST_INFO));

	ZeroMemory(mVirtualAddress, sizeof(mVirtualAddress));
	ZeroMemory(mRealAddress, sizeof(mRealAddress));

	mSideInGame = SIDE_NONE;

	mVirtualPort = 0;
	mRealPort = 0;
	mStatus = US_NONE;
	mEnteredRoom = NULL;
	mIsConnected = false;
	//mIsInCourt = false;

	//mNumAlive = 0;
	mNumShotDown = 0;
}

CConnectedUser::~CConnectedUser(void)
{
}

bool CConnectedUser::Begin(void)
{
	CThreadSync Sync;

	ZeroMemory(&mUserAccountInfo, sizeof(S_USER_ACCOUNT_INFO));
	ZeroMemory(&mUserEquipmentInfo, sizeof(S_USER_EQUIPMENT_INFO));
	ZeroMemory(&mUserChestInfo, sizeof(S_USER_CHEST_INFO));

	ZeroMemory(mVirtualAddress, sizeof(mVirtualAddress));
	ZeroMemory(mRealAddress, sizeof(mRealAddress));

	mSideInGame = SIDE_NONE;

	mVirtualPort = 0;
	mRealPort = 0;
	mStatus = US_NONE;
	mEnteredRoom = NULL;
	mOppoUser = NULL; // 对手连接地址
	mIsConnected = false;
	//mIsInCourt = false;

	//mNumAlive = 0;
	mNumShotDown = 0;

	return CPacketSession::Begin();
}

bool CConnectedUser::End(void)
{
	CThreadSync Sync;

	ZeroMemory(&mUserAccountInfo, sizeof(S_USER_ACCOUNT_INFO));
	ZeroMemory(&mUserEquipmentInfo, sizeof(S_USER_EQUIPMENT_INFO));
	ZeroMemory(&mUserChestInfo, sizeof(S_USER_CHEST_INFO));

	ZeroMemory(mVirtualAddress, sizeof(mVirtualAddress));
	ZeroMemory(mRealAddress, sizeof(mRealAddress));

	mSideInGame = SIDE_NONE;

	mVirtualPort = 0;
	mRealPort = 0;
	mStatus = US_NONE;
	mEnteredRoom = NULL;
	mOppoUser = NULL; // 对手连接地址
	mIsConnected = false;

	mNumShotDown = 0;

	return CPacketSession::End();
}

bool CConnectedUser::GameEnd()
{
	// 先判断本次游戏胜负 并更新账户信息
	int numShotDownOwn = this->GetNumShotDown();
	int numShotDownOppo = mOppoUser->GetNumShotDown();

	int userIdOwn = this->GetUserAccountInfo()->USER_ID;
	int userIdOppo = mOppoUser->GetUserAccountInfo()->USER_ID;

	//if(numAliveOwn == 0 && numAliveOppo == 0) {
	//	// 平局
	//	printf("平局\n");
	//} else if(numAliveOwn == 0 && numAliveOppo > 0) {
	//	// 对手获胜
	//	printf("userID = %d 的玩家获胜\n", userIdOppo);
	//} else if(numAliveOwn > 0 && numAliveOppo == 0) {
	//	// 本方获胜
	//	printf("userID = %d 的玩家获胜\n", userIdOwn);
	//} else if(numAliveOwn == numAliveOppo) {
	//	if(numShotDownOwn == numShotDownOppo) {
	//		// 平局
	//		printf("平局\n");
	//	} else if(numShotDownOwn > numShotDownOppo) {
	//		// 本方获胜
	//		printf("userID = %d 的玩家获胜\n", userIdOwn);
	//	} else if(numShotDownOwn < numShotDownOppo) {
	//		// 对手获胜
	//		printf("userID = %d 的玩家获胜\ns", userIdOppo);
	//	}
	//}

	mStatus = US_CHANNEL_ENTERED; // 用户当前状态
	mEnteredRoom = NULL; // 用户当前登录房间的地址

	mSideInGame = SIDE_NONE;

	mNumShotDown = 0;

	return true;
}

bool CConnectedUser::Reload(SOCKET listenSocket)
{
	CThreadSync Sync;

	printf("CConnectedUser::Reload\n");

	// 结束当前 CConnectedUser
	End();

	// 重新初始化 CConnectedUser
	if (!Begin())
		return false;

	// 重新设置 CConnectedUser 网络配置
	// 等待下一个连接用户
	if (!CNetworkSession::Accept(listenSocket))
		return false;

	return true;
}

void CConnectedUser::loadUserInfoFromDB(void* userInfo)
{
	CThreadSync Sync;

	S_USER_INFO* UserInfo = new S_USER_INFO();
	memcpy(UserInfo, userInfo, sizeof(S_USER_INFO));

	// mUserAccountInfo
	mUserAccountInfo.USER_ID = UserInfo->USER_ID;
	memcpy(mUserAccountInfo.NICKNAME, UserInfo->NICKNAME, 32);
	memcpy(mUserAccountInfo.PASSWORD, UserInfo->PASSWORD, 32);
	memcpy(mUserAccountInfo.BIND_ACCOUNT_TYPE, UserInfo->BIND_ACCOUNT_TYPE, 32);
	memcpy(mUserAccountInfo.BIND_ACCOUNT, UserInfo->BIND_ACCOUNT, 32);
	memcpy(mUserAccountInfo.CELLPHONE_NO, UserInfo->CELLPHONE_NO, 32);
	mUserAccountInfo.INTEGRAL_POINTS = UserInfo->INTEGRAL_POINTS;
	mUserAccountInfo.LEVEL = UserInfo->LEVEL;
	mUserAccountInfo.DIAMONDS = UserInfo->DIAMONDS;
	mUserAccountInfo.COINS = UserInfo->COINS;
	mUserAccountInfo.WIN_TIMES = UserInfo->WIN_TIMES;
	mUserAccountInfo.BATTLE_TIMES = UserInfo->BATTLE_TIMES;
	mUserAccountInfo.WIN_RATE = UserInfo->WIN_RATE;
	mUserAccountInfo.SHOT_DOWN_PLANES = UserInfo->SHOT_DOWN_PLANES;
	mUserAccountInfo.SHOT_DOWN_JUNIOR_PLANES = UserInfo->SHOT_DOWN_JUNIOR_PLANES;
	mUserAccountInfo.SHOT_DOWN_MIDDLE_PLANES = UserInfo->SHOT_DOWN_MIDDLE_PLANES;
	mUserAccountInfo.SHOT_DOWN_SENIOR_PLANES = UserInfo->SHOT_DOWN_SENIOR_PLANES;
	mUserAccountInfo.SESSION_ID = UserInfo->SESSION_ID;

	// mUserEquipmentInfo
	if(UserInfo->NUM_OF_JUNIOR_PLANE_CARD > 0 && UserInfo->LEVEL_JUNIOR_PLANE == 0) {
		UserInfo->LEVEL_JUNIOR_PLANE = 1;
	}

	if(UserInfo->NUM_OF_MIDDLE_PLANE_CARD > 0 && UserInfo->LEVEL_MIDDLE_PLANE == 0) {
		UserInfo->LEVEL_MIDDLE_PLANE = 1;
	}

	if(UserInfo->NUM_OF_SENIOR_PLANE_CARD > 0 && UserInfo->LEVEL_SENIOR_PLANE == 0) {
		UserInfo->LEVEL_SENIOR_PLANE = 1;
	}

	if(UserInfo->NUM_OF_JUNIOR_STEALTH_PLANE_CARD > 0 && UserInfo->LEVEL_JUNIOR_STEALTH_PLANE == 0) {
		UserInfo->LEVEL_JUNIOR_STEALTH_PLANE = 1;
	}

	if(UserInfo->NUM_OF_MIDDLE_STEALTH_PLANE_CARD > 0 && UserInfo->LEVEL_MIDDLE_STEALTH_PLANE == 0) {
		UserInfo->LEVEL_MIDDLE_STEALTH_PLANE = 1;
	}

	if(UserInfo->NUM_OF_SENIOR_STEALTH_PLANE_CARD > 0 && UserInfo->LEVEL_SENIOR_STEALTH_PLANE == 0) {
		UserInfo->LEVEL_SENIOR_STEALTH_PLANE = 1;
	}

	if(UserInfo->NUM_OF_SENIOR_ANTI_STEALTH_RADAR_CARD > 0 && UserInfo->LEVEL_SENIOR_ANTI_STEALTH_RADAR == 0) {
		UserInfo->LEVEL_SENIOR_ANTI_STEALTH_RADAR = 1;
	}

	if(UserInfo->NUM_OF_MIDDLE_ANTI_STEALTH_RADAR_CARD > 0 && UserInfo->LEVEL_MIDDLE_ANTI_STEALTH_RADAR == 0) {
		UserInfo->LEVEL_MIDDLE_ANTI_STEALTH_RADAR = 1;
	}

	if(UserInfo->NUM_OF_JUNIOR_ANTI_STEALTH_RADAR_CARD > 0 && UserInfo->LEVEL_JUNIOR_ANTI_STEALTH_RADAR == 0) {
		UserInfo->LEVEL_JUNIOR_ANTI_STEALTH_RADAR = 1;
	}

	if(UserInfo->NUM_OF_SENIOR_MISSILE_CARD > 0 && UserInfo->LEVEL_SENIOR_MISSILE == 0) {
		UserInfo->LEVEL_SENIOR_MISSILE = 1;
	}

	if(UserInfo->NUM_OF_MIDDLE_MISSILE_CARD > 0 && UserInfo->LEVEL_MIDDLE_MISSILE == 0) {
		UserInfo->LEVEL_MIDDLE_MISSILE = 1;
	};
	if(UserInfo->NUM_OF_JUNIOR_MISSILE_CARD > 0 && UserInfo->LEVEL_JUNIOR_MISSILE == 0) {
		UserInfo->LEVEL_JUNIOR_MISSILE = 1;
	}

	if(UserInfo->NUM_OF_SENIOR_RADAR_CARD > 0 && UserInfo->LEVEL_SENIOR_RADAR == 0) {
		UserInfo->LEVEL_SENIOR_RADAR = 1;
	}

	if(UserInfo->NUM_OF_MIDDLE_RADAR_CARD > 0 && UserInfo->LEVEL_MIDDLE_RADAR == 0) {
		UserInfo->LEVEL_MIDDLE_RADAR = 1;
	}

	if(UserInfo->NUM_OF_JUNIOR_RADAR_CARD > 0 && UserInfo->LEVEL_JUNIOR_RADAR == 0) {
		UserInfo->LEVEL_JUNIOR_RADAR = 1;
	}

	if(UserInfo->NUM_OF_SENIOR_CANNONBALL_CARD > 0 && UserInfo->LEVEL_SENIOR_CANNONBALL == 0) {
		UserInfo->LEVEL_SENIOR_CANNONBALL = 1;
	}

	if(UserInfo->NUM_OF_MIDDLE_CANNONBALL_CARD > 0 && UserInfo->LEVEL_MIDDLE_CANNONBALL == 0) {
		UserInfo->LEVEL_MIDDLE_CANNONBALL = 1;
	}

	if(UserInfo->NUM_OF_JUNIOR_CANNONBALL_CARD > 0 && UserInfo->LEVEL_JUNIOR_CANNONBALL == 0) {
		UserInfo->LEVEL_JUNIOR_CANNONBALL = 1;
	}

	mUserEquipmentInfo.USER_ID = UserInfo->USER_ID;
	memcpy(mUserEquipmentInfo.PASSWORD, UserInfo->PASSWORD, 32);
	mUserEquipmentInfo.LEVEL = UserInfo->LEVEL;
	mUserEquipmentInfo.JUNIOR_PLANE_CARD_NUM = UserInfo->NUM_OF_JUNIOR_PLANE_CARD;
	mUserEquipmentInfo.MIDDLE_PLANE_CARD_NUM = UserInfo->NUM_OF_MIDDLE_PLANE_CARD;
	mUserEquipmentInfo.SENIOR_PLANE_CARD_NUM = UserInfo->NUM_OF_SENIOR_PLANE_CARD;
	mUserEquipmentInfo.JUNIOR_STEALTH_PLANE_CARD_NUM = UserInfo->NUM_OF_JUNIOR_STEALTH_PLANE_CARD;
	mUserEquipmentInfo.MIDDLE_STEALTH_PLANE_CARD_NUM = UserInfo->NUM_OF_MIDDLE_STEALTH_PLANE_CARD;
	mUserEquipmentInfo.SENIOR_STEALTH_PLANE_CARD_NUM = UserInfo->NUM_OF_SENIOR_STEALTH_PLANE_CARD;
	mUserEquipmentInfo.SENIOR_ANTI_STEALTH_RADAR_CARD_NUM = UserInfo->NUM_OF_SENIOR_ANTI_STEALTH_RADAR_CARD;
	mUserEquipmentInfo.MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM = UserInfo->NUM_OF_MIDDLE_ANTI_STEALTH_RADAR_CARD;
	mUserEquipmentInfo.JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM = UserInfo->NUM_OF_JUNIOR_ANTI_STEALTH_RADAR_CARD;
	mUserEquipmentInfo.SENIOR_MISSILE_CARD_NUM = UserInfo->NUM_OF_SENIOR_MISSILE_CARD;
	mUserEquipmentInfo.MIDDLE_MISSILE_CARD_NUM = UserInfo->NUM_OF_MIDDLE_MISSILE_CARD;
	mUserEquipmentInfo.JUNIOR_MISSILE_CARD_NUM = UserInfo->NUM_OF_JUNIOR_MISSILE_CARD;
	mUserEquipmentInfo.SENIOR_RADAR_CARD_NUM = UserInfo->NUM_OF_SENIOR_RADAR_CARD;
	mUserEquipmentInfo.MIDDLE_RADAR_CARD_NUM = UserInfo->NUM_OF_MIDDLE_RADAR_CARD;
	mUserEquipmentInfo.JUNIOR_RADAR_CARD_NUM = UserInfo->NUM_OF_JUNIOR_RADAR_CARD;
	mUserEquipmentInfo.SENIOR_CANNONBALL_CARD_NUM = UserInfo->NUM_OF_SENIOR_CANNONBALL_CARD;
	mUserEquipmentInfo.MIDDLE_CANNONBALL_CARD_NUM = UserInfo->NUM_OF_MIDDLE_CANNONBALL_CARD;
	mUserEquipmentInfo.JUNIOR_CANNONBALL_CARD_NUM = UserInfo->NUM_OF_JUNIOR_CANNONBALL_CARD;

	mUserEquipmentInfo.JUNIOR_PLANE_LEVEL = UserInfo->LEVEL_JUNIOR_PLANE;
	mUserEquipmentInfo.MIDDLE_PLANE_LEVEL = UserInfo->LEVEL_MIDDLE_PLANE;
	mUserEquipmentInfo.SENIOR_PLANE_LEVEL = UserInfo->LEVEL_SENIOR_PLANE;
	mUserEquipmentInfo.JUNIOR_STEALTH_PLANE_LEVEL = UserInfo->LEVEL_JUNIOR_STEALTH_PLANE;
	mUserEquipmentInfo.MIDDLE_STEALTH_PLANE_LEVEL = UserInfo->LEVEL_MIDDLE_STEALTH_PLANE;
	mUserEquipmentInfo.SENIOR_STEALTH_PLANE_LEVEL = UserInfo->LEVEL_SENIOR_STEALTH_PLANE;
	mUserEquipmentInfo.SENIOR_ANTI_STEALTH_RADAR_LEVEL = UserInfo->LEVEL_SENIOR_ANTI_STEALTH_RADAR;
	mUserEquipmentInfo.MIDDLE_ANTI_STEALTH_RADAR_LEVEL = UserInfo->LEVEL_MIDDLE_ANTI_STEALTH_RADAR;
	mUserEquipmentInfo.JUNIOR_ANTI_STEALTH_RADAR_LEVEL = UserInfo->LEVEL_JUNIOR_ANTI_STEALTH_RADAR;
	mUserEquipmentInfo.SENIOR_MISSILE_LEVEL = UserInfo->LEVEL_SENIOR_MISSILE;
	mUserEquipmentInfo.MIDDLE_MISSILE_LEVEL = UserInfo->LEVEL_MIDDLE_MISSILE;
	mUserEquipmentInfo.JUNIOR_MISSILE_LEVEL = UserInfo->LEVEL_JUNIOR_MISSILE;
	mUserEquipmentInfo.SENIOR_RADAR_LEVEL = UserInfo->LEVEL_SENIOR_RADAR;
	mUserEquipmentInfo.MIDDLE_RADAR_LEVEL = UserInfo->LEVEL_MIDDLE_RADAR;
	mUserEquipmentInfo.JUNIOR_RADAR_LEVEL = UserInfo->LEVEL_JUNIOR_RADAR;
	mUserEquipmentInfo.SENIOR_CANNONBALL_LEVEL = UserInfo->LEVEL_SENIOR_CANNONBALL;
	mUserEquipmentInfo.MIDDLE_CANNONBALL_LEVEL = UserInfo->LEVEL_MIDDLE_CANNONBALL;
	mUserEquipmentInfo.JUNIOR_CANNONBALL_LEVEL = UserInfo->LEVEL_JUNIOR_CANNONBALL;

	// mUserChestInfo
	mUserChestInfo.USER_ID = UserInfo->USER_ID;
	mUserChestInfo.STAR_NUM = UserInfo->STAR_NUM;
	mUserChestInfo.WIN_REWARDS_1_NUM = UserInfo->WIN_REWARDS_1_NUM;
	mUserChestInfo.WIN_REWARDS_2_NUM = UserInfo->WIN_REWARDS_2_NUM;
	mUserChestInfo.WIN_REWARDS_3_NUM = UserInfo->WIN_REWARDS_3_NUM;
	mUserChestInfo.WIN_REWARDS_4_NUM = UserInfo->WIN_REWARDS_4_NUM;
	mUserChestInfo.WIN_REWARDS_5_NUM = UserInfo->WIN_REWARDS_5_NUM;
	mUserChestInfo.STAR_REWARDS_NUM = UserInfo->STAR_REWARDS_NUM;
	mUserChestInfo.FREE_REWARDS_NUM = UserInfo->FREE_REWARDS_NUM;
	mUserChestInfo.WIN_REWARDS_1_UNLOCK_TIME = UserInfo->WIN_REWARDS_1_UNLOCK_TIME;
	mUserChestInfo.WIN_REWARDS_2_UNLOCK_TIME = UserInfo->WIN_REWARDS_2_UNLOCK_TIME;
	mUserChestInfo.WIN_REWARDS_3_UNLOCK_TIME = UserInfo->WIN_REWARDS_3_UNLOCK_TIME;
	mUserChestInfo.WIN_REWARDS_4_UNLOCK_TIME = UserInfo->WIN_REWARDS_4_UNLOCK_TIME;
	mUserChestInfo.WIN_REWARDS_5_UNLOCK_TIME = UserInfo->WIN_REWARDS_5_UNLOCK_TIME;
	mUserChestInfo.LAST_STAR_REWARDS_ACQUIRE_TIME = UserInfo->LAST_STAR_REWARDS_ACQUIRE_TIME;
	mUserChestInfo.LAST_FREE_REWARDS_ACQUIRE_TIME = UserInfo->LAST_FREE_REWARDS_ACQUIRE_TIME;

	delete UserInfo;
}

void CConnectedUser::loadUserEquipmentInfoWithDefaultValue()
{
	CThreadSync Sync;

	memset(&mUserEquipmentInfo, 0, sizeof(S_USER_EQUIPMENT_INFO));
	mUserEquipmentInfo.USER_ID = mUserAccountInfo.USER_ID;
	memcpy(mUserEquipmentInfo.PASSWORD, mUserAccountInfo.PASSWORD, 32);
	mUserEquipmentInfo.LEVEL = 1;
	mUserEquipmentInfo.JUNIOR_PLANE_LEVEL = 1;
	mUserEquipmentInfo.JUNIOR_CANNONBALL_LEVEL = 1;
}
















