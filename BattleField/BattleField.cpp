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
	// 武器标签计数器置零
	mWeaponTagCounter = 0;
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
	// 武器标签计数器置零
	mWeaponTagCounter = 0;
}

// 对战开始
void CBattleField::BattleStart()
{
	// 存储对战双方兵力的动态数组
	mTroopsInBlueTeam.clear();
	mTroopsInRedTeam.clear();

	// 将当前游戏状态设置为游戏中
	mStatus = BATTLE_STATUS_IN_PROGRESSING;

	// 设置游戏时长
	mRemainGameTime = DEFALUT_GAME_TIME;
	// 武器标签计数器置零
	mWeaponTagCounter = 0;
}

Weapon CBattleField::InputTroops(ENUM_WEAPON_TYPE weaponType, ENUM_TROOPS troopsIn, float posX, float posY)
{
	printf("BattleField::inputTroopsBlue\n");

	// 测试时使用的武器属性参数 
	// 应当更改为用户从数据库中读取的参数
	ENUM_WEAPON_TYPE WP_TYPE = weaponType;
	int LEVEL = 1;
	float SPEED = 180;
	int RANGE_DEC = 5;
	int DP = 10;
	int HP = 100;
	int RANGE_FIRE = 5;

	S_PROPERTY_WP propertyWp = S_PROPERTY_WP{ WP_TYPE, LEVEL, SPEED, RANGE_DEC, DP, HP, RANGE_FIRE };

	Weapon Weapon;

	// 武器在战场中的标签
	int WeaponTag = mWeaponTagCounter;
	// 重新计算武器在战场中的纵坐标
	if (troopsIn == TROOPS_BLUE) {
		// 蓝方从下方向上飞行
		posY = posY;
	}
	else if (troopsIn == TROOPS_RED) {
		// 红方从上方向下飞行
		posY = 2060 - posY;
	}

	Weapon.init(troopsIn, propertyWp, posX, posY, WeaponTag);
	mWeaponTagCounter++;

	// 将武器加入对应的蓝方或者红方
	if (troopsIn == TROOPS_BLUE) {
		mTroopsInBlueTeam.push_back(Weapon);
	}
	else if (troopsIn == TROOPS_RED) {
		mTroopsInRedTeam.push_back(Weapon);
	}

	return Weapon;
}

// 战场态势刷新
ENUM_BATTLE_STATUS CBattleField::UpdateBattleFieldSituation()
{
	printf("BattleField::updateBattleFieldSituation time = %d\n", mRemainGameTime);

	// 刷新蓝方兵力位置
	for (int index = 0; index < mTroopsInBlueTeam.size(); index++) {
		mTroopsInBlueTeam[index].Move();
	}

	// 刷新红方兵力位置
	for (int index = 0; index < mTroopsInRedTeam.size(); index++) {
		mTroopsInRedTeam[index].Move();
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