#include "stdafx.h"
#include "BattleSituationMapCell.h"

void CBattleSituationMapCell::init(Vec2 coordinate)
{
	// 设置战场方格的仿真坐标
	mCoordinate = coordinate;

	// 重置位于方格兵力的武器类型
	mWeaponType = WP_TYPE_NONE;
	// 重置位于方格兵力的武器标签
	mWeaponTag = TAG_NO_WEAPON_IN;

	// 重置对战双方分别面对的战争迷雾
	mFogAgainstBlueEn = true;
	mFogAgainstRedEn = true;

	// 清空电磁波传输动态数组
	mHertzWaveVec.clear();
}