#include "stdafx.h"
#include "Weapon.h"

// 构造函数
Weapon::Weapon()
{
	mTroopsIn = TROOPS_NONE;
	memset(&mPropertyWp, 0, sizeof(S_PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = STATUS_WP_NONE;
}

// 析构函数
Weapon::~Weapon()
{
	mTroopsIn = TROOPS_NONE;
	memset(&mPropertyWp, 0, sizeof(S_PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = STATUS_WP_NONE;
}

// 初始化函数
void Weapon::init(ENUM_TROOPS troops, S_PROPERTY_WP propertyWp, float posX, float posY)
{
	mTroopsIn = troops;
	memcpy(&mPropertyWp, &propertyWp, sizeof(S_PROPERTY_WP));
	mPosX = posX;
	mPosY = posY;

	// 建筑类武器将状态设置为 STATUS_WP_STOP 
	// 飞机类武器将状态设置为 STATUS_WP_MOVE_FORWARD
	if (propertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		mStatus = STATUS_WP_STOP;
	}
	else {
		mStatus = STATUS_WP_MOVE_FORWARD;
	}
}

// 武器移动
void Weapon::Move()
{
	// 武器类型无效 或 武器为建筑类 不能移动 函数返回
	if (mPropertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		return;
	}

	switch (mTroopsIn)
	{
	// 蓝方所属兵力 自战场下方向上运动
	case TROOPS_BLUE:
		this->mPosY += mPropertyWp.SPEED;
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

}

// 遭到攻击
int Weapon::BeAttacked(int DP)
{
	return mPropertyWp.HP = max(0, mPropertyWp.HP - DP);
}