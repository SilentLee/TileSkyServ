#pragma once
#include <vector>
#include "MultiThreadSync.h"
#include "CriticalSection.h"
#include "Weapon.h"
#include "BattleSituationMapCell.h"

using namespace std;

#define DEFALUT_GAME_TIME 30000

typedef enum ENUM_BATTLE_STATUS {
	BATTLE_STATUS_IN_PREPARING,
	BATTLE_STATUS_IN_PROGRESSING,
	BATTLE_STATUS_BLUE_WIN,
	BATTLE_STATUS_RED_WIN,
	BATTLE_STATUS_DRAW
};

class CBattleField : public CMultiThreadSync<CBattleField>
{
// 战场态势仿真地图
private:

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
	// 兵力投入战场 返回加入战场的武器
	Weapon InputTroops(ENUM_WEAPON_TYPE weaponType, ENUM_TROOPS troopsIn, float posX, float posY);
	// 刷新战场态势 同时返回游戏状态
	ENUM_BATTLE_STATUS UpdateBattleFieldSituation();
	// 战场中武器标签计数器
	int mWeaponTagCounter;

// 存取函数
public:
	// 当前战场状态
	void SetStatus(ENUM_BATTLE_STATUS status) { CThreadSync Sync; mStatus = status; };
	ENUM_BATTLE_STATUS GetStatus() { CThreadSync Sync; return mStatus; };
	// 当前游戏计时
	bool DecreaseRemainGameTime() { CThreadSync Sync; mRemainGameTime = max(0, mRemainGameTime--); return mRemainGameTime ? true : false; };
	int GetRemainGameTime() { CThreadSync Sync; return mRemainGameTime; };

	// 获取指定兵力
	Weapon* getTroopInBlueTeam(int index) { CThreadSync Sync; return &mTroopsInBlueTeam[index]; };
	Weapon* getTroopInRedTeam(int index) { CThreadSync Sync; return &mTroopsInRedTeam[index]; };
	// 获取兵力数量
	int getCountOfTroopsInBlueTeam() { CThreadSync Sync; return mTroopsInBlueTeam.size(); };
	int getCountOfTroopsInRedTeam() { CThreadSync Sync; return mTroopsInRedTeam.size(); };

	// 设置战场中武器标签计数器
	bool increaseWeaponTagCounter() { CThreadSync Sync; mWeaponTagCounter = mWeaponTagCounter++; };
	int getWeaponTagCounter() { CThreadSync Sync; return mWeaponTagCounter; };
};