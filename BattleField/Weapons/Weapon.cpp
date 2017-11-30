#include "stdafx.h"
#include "Weapon.h"

// 构造函数
Weapon::Weapon()
{
	mSideInGame = SIDE_NONE;
	memset(&mPropertyWp, 0, sizeof(PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = STATUS_WP_NONE;
}

// 析构函数
Weapon::~Weapon()
{
	mSideInGame = SIDE_NONE;
	memset(&mPropertyWp, 0, sizeof(PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = STATUS_WP_NONE;
}

// 初始化函数
void Weapon::init(int sideInGame, PROPERTY_WP propertyWp, float posX, float posY)
{
	mSideInGame = sideInGame;
	memcpy(&mPropertyWp, &propertyWp, sizeof(PROPERTY_WP));
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

// 存取函数
// 读取
void Weapon::setSideInGame(int sideInGame)
{
	mSideInGame = sideInGame;
}

void Weapon::setProperty(PROPERTY_WP propertyWp)
{
	memcpy(&mPropertyWp, &propertyWp, sizeof(PROPERTY_WP));
}

void Weapon::setPosX(float posX)
{
	mPosX = posX;
}

void Weapon::setPosY(float posY)
{
	mPosY = posY;
}

void Weapon::setStatus(int status)
{
	mStatus = status;
}

// 写入
int Weapon::getSideInGame()
{
	return mSideInGame;
}

PROPERTY_WP Weapon::getProperty()
{
	return mPropertyWp;
}

float Weapon::getPosX()
{
	return mPosX;
}

float Weapon::getPosY()
{
	return mPosY;
}

int Weapon::getStatus()
{
	return mStatus;
}

// 武器移动
void Weapon::move()
{
	// 武器类型无效 或 武器为建筑类 不能移动 函数返回
	if (mPropertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		return;
	}

	switch (mSideInGame)
	{
	// 蓝方所属兵力 自战场下方向上运动
	case SIDE_BLUE:
		this->mPosY += mPropertyWp.SPEED;
		break;

	// 红方所属兵力 自战场上方向下运动
	case SIDE_RED:
		mPosY = max(0.0f, mPosY - mPropertyWp.SPEED);
		break;
	}
}

// 武器攻击
void Weapon::attack()
{

}

// 遭到攻击
int Weapon::beAttacked(int DP)
{
	return mPropertyWp.HP = max(0, mPropertyWp.HP - DP);
}