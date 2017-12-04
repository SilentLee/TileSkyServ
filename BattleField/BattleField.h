#pragma once
#include <vector>
#include "Weapon.h"

using namespace std;

#define DEFALUT_GAME_TIME 30000

typedef enum ENUM_BATTLE_STATUS {
	BATTLE_STATUS_IN_PREPARING,
	BATTLE_STATUS_IN_PROGRESSING,
	BATTLE_STATUS_BLUE_WIN,
	BATTLE_STATUS_RED_WIN,
	BATTLE_STATUS_DRAW
};

class CBattleField
{
private:
	// 存储对战双方兵力的动态数组
	vector<Weapon> mTroopsInBlueTeam;
	vector<Weapon> mTroopsInRedTeam;

	// 当前游戏状态
	ENUM_BATTLE_STATUS mStatus;
	// 剩余游戏时间
	int mRemainGameTime;

public:
	// 战场初始化函数
	void Begin();
	// 战场重置函数
	void End();
	// 对战开始
	void BattleStart();
public:
	// 蓝军兵力投入战场
	void InputTroopsBlue(int weaponType, float posX, float posY);
	// 红军兵力投入战场
	void InputTroopsRed(int weaponType, float posX, float posY);
	// 刷新战场态势 同时返回游戏状态
	ENUM_BATTLE_STATUS UpdateBattleFieldSituation();

// 存取函数
public:
	// 当前战场状态
	void SetStatus(ENUM_BATTLE_STATUS status) { mStatus = status; };
	ENUM_BATTLE_STATUS GetStatus() { return mStatus; };
	// 当前游戏计时
	bool DecreaseRemainGameTime() { mRemainGameTime = max(0, mRemainGameTime--); return mRemainGameTime ? true : false; };
	int GetRemainGameTime() { return mRemainGameTime; };

	// 获取指定兵力
	Weapon* getTroopInBlueTeam(int index) { return &mTroopsInBlueTeam[index]; };
	Weapon* getTroopInRedTeam(int index) { return &mTroopsInRedTeam[index]; };
	// 获取兵力数量
	int getCountOfTroopsInBlueTeam() { return mTroopsInBlueTeam.size(); };
	int getCountOfTroopsInRedTeam() { return mTroopsInRedTeam.size(); };
};