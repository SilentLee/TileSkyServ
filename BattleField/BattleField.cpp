#include "stdafx.h"
#include "BattleField.h"

// 战场初始化函数
void CBattleField::Begin()
{
	// 存储对战双方兵力的动态数组
	mTroopsInBlueTeam.clear();
	mTroopsInRedTeam.clear();
	
	// 当前游戏状态
	mStatus = BATTLE_STATUS_IN_PREPARING;
	// 剩余游戏时间
	mRemainGameTime = DEFALUT_GAME_TIME;
}

// 战场重置函数
void CBattleField::End()
{
	// 存储对战双方兵力的动态数组
	mTroopsInBlueTeam.clear();
	mTroopsInRedTeam.clear();

	// 当前游戏状态
	mStatus = BATTLE_STATUS_IN_PREPARING;
	// 剩余游戏时间
	mRemainGameTime = DEFALUT_GAME_TIME;
}

// 投入战场兵力
void CBattleField::InputTroops(int weaponType, int sideInGame, float posX, float posY)
{
	printf("BattleField::inputTroops\n");

	// 测试时使用的武器属性参数 
	// 应当更改为用户从数据库中读取的参数
	int WP_TYPE = weaponType;
	int LEVEL = 1;
	float SPEED = 10;
	int RANGE_DEC = 5;
	int DP = 10;
	int HP = 100;
	int RANGE_FIRE = 5;

	PROPERTY_WP propertyWp = PROPERTY_WP{ WP_TYPE, LEVEL, SPEED, RANGE_DEC, DP, HP, RANGE_FIRE };

	Weapon weapon;
	weapon.init(sideInGame, propertyWp, posX, posY);

	if (sideInGame == SIDE_BLUE) {
		mTroopsInBlueTeam.push_back(weapon);
	}
	else {
		mTroopsInRedTeam.push_back(weapon);
	}
}

// 战场态势刷新
BATTLE_STATUS CBattleField::UpdateBattleFieldSituation()
{
	printf("BattleField::updateBattleFieldSituation\n");

	// 刷新蓝方兵力位置
	for (int index = 0; index < mTroopsInBlueTeam.size(); index++) {
		mTroopsInBlueTeam[index].move();
	}

	// 刷新红方兵力位置
	for (int index = 0; index < mTroopsInRedTeam.size(); index++) {
		mTroopsInRedTeam[index].move();
	}

	// 游戏计时减 1
	DecreaseRemainGameTime();

	// 当计时到 0 时 游戏结束
	if (mRemainGameTime == 0) {
		mStatus = BATTLE_STATUS_DRAW;
	}
	// 计时尚未结束
	else {
		mStatus = BATTLE_STATUS_IN_PROGRESSING;
	}

	return mStatus;
}