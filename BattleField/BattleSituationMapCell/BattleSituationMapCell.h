#pragma once
#include "Weapon.h"
#include <vector>

// 战场模拟地图方格宽度
#define WIDTH_OF_BATTLE_SIMULATION_MAP_CELL			27.0f
// 战场态势仿真地图宽度
#define WIDTH_OF_BATTLE_SIMULATION_MAP							40
// 战场态势仿真地图高度
#define HEIGHT_OF_BATTLE_SIMULATION_MAP						60
// 战场态势显示地图宽度
#define WIDTH_OF_BATTLE_DISPLAY_MAP								1080.0f
// 战场态势显示地图高度
#define HEIGHT_OF_BATTLE_DISPLAY_MAP								1620.0f

// 武器标签初始值、
#define TAG_NO_WEAPON_IN															-1

typedef struct _S_HERTZ_WAVE {
	// 电平
	float LEVEL;
	// 发射平台武器标签
	int TAG;
	// 发射方向
	int DIRECTION_FROM;
} S_HERTZ_WAVE;

typedef struct _Vec2 {
	float x;
	float y;
} Vec2;

class CBattleSituationMapCell
{
private:
	// 地图方格坐标
	Vec2 mCoordinate;
	// 处于当前方格空间中的武器类型
	ENUM_WEAPON_TYPE mWeaponType;
	// 处于当前方格空间中的武器标签
	int mWeaponTag;
	// 蓝方面对的战争迷雾使能
	bool mFogAgainstBlueEn;
	// 红方面对的战争迷雾使能
	bool mFogAgainstRedEn;
	// 充斥于当前方格空间中的电磁波序列
	vector<S_HERTZ_WAVE> mHertzWaveVec;

public:
	void init(Vec2 coordinate);

// 存取函数
public:
	void SetCoordinate(Vec2 coordinate) { mCoordinate = coordinate; };
	Vec2 GetCoordinate() { return mCoordinate; };

	void SetWeaponType(ENUM_WEAPON_TYPE weaponType) { mWeaponType = weaponType; };
	ENUM_WEAPON_TYPE GetWeaponTYpe() { return mWeaponType; };

	void SetWeaponTag(int weaponTag) { mWeaponTag = weaponTag; };
	int GetWeaponTag() { return mWeaponTag; };
	void resetWeaponTag() { mWeaponTag = TAG_NO_WEAPON_IN; };

	void SetFogAgainstBlueEn(bool fogAgainstBlueEn) { mFogAgainstBlueEn = fogAgainstBlueEn; };
	int GetFogAgainstBlueEn() { return mFogAgainstBlueEn; };

	void SegFogAgainstRedEn(bool fogAgainstRedEn) { mFogAgainstRedEn = fogAgainstRedEn; };
	int GetFogAgainstRedEn() { return mFogAgainstRedEn; };
};