#include "stdafx.h"
#include "Weapon.h"
//sdlfkjdsafjlsadjfklkskjdlkfjslkfdjlksdjflksjdlfkjsklf
// 构造函数
Weapon::Weapon()
{
	mTroopsIn = TROOPS_NONE;
	memset(&mPropertyWp, 0, sizeof(S_PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = WP_STATUS_NONE;
	mWeaponTag = 0;
}

// 析构函数
Weapon::~Weapon()
{
	mTroopsIn = TROOPS_NONE;
	memset(&mPropertyWp, 0, sizeof(S_PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = WP_STATUS_NONE;
	mWeaponTag = 0;
}

// 初始化函数
void Weapon::init(ENUM_TROOPS troops, S_PROPERTY_WP propertyWp, float posX, float posY, int weaponTag)
{
	CThreadSync Sync;

	mTroopsIn = troops;
	memcpy(&mPropertyWp, &propertyWp, sizeof(S_PROPERTY_WP));
	mPosX = posX;
	mPosY = posY;

	// 设置武器标签
	mWeaponTag = weaponTag;

	// 建筑类武器将状态设置为 STATUS_WP_STOP 
	// 飞机类武器将状态设置为 STATUS_WP_MOVE_FORWARD
	if (propertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		mStatus = WP_STATUS_STOP;
	}
	else {
		mStatus = WP_STATUS_MOVING;
	}
}

// 武器移动
void Weapon::Move()
{
	CThreadSync Sync;

	// 武器类型无效 或 武器为建筑类 不能移动 函数返回
	if (mPropertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		return;
	}

	switch (mTroopsIn)
	{
	// 蓝方所属兵力 自战场下方向上运动
	case TROOPS_BLUE:
		mPosY = min(HEIGHT_OF_BATTLE_DISPLAY_MAP, mPosY + mPropertyWp.SPEED);
		break;

	// 红方所属兵力 自战场上方向下运动
	case TROOPS_RED:
		mPosY = max(0.0f, mPosY - mPropertyWp.SPEED);
		break;
	}
}

// 武器攻击
void Weapon::Attack()
{
	CThreadSync Sync;
}

// 遭到攻击
int Weapon::BeAttacked(int DP)
{
	CThreadSync Sync;

	return mPropertyWp.HP = max(0, mPropertyWp.HP - DP);
}