#pragma once
#include "string.h"
#include <algorithm>

using namespace std;

typedef enum ENUM_WEAPON_TYPE {
	WP_TYPE_NONE = 0,

	/************以下为建筑类型武器 不能移动*************/
	WP_TYPE_ANTI_AIRCRAFT_MISSILE,
	WP_TYPE_FLAK, // 高射炮
	WP_TYPE_RADAR,

	/************以下为飞机类型武器 可以移动*************/
	WP_TYPE_FIGHTER_PLANE,
	WP_TYPE_BOMBING_PLANE,
	WP_TYPE_EARLY_WARNING_PLANE,
	WP_TYPE_JAMMING_PLANE,
	WP_TYPE_8,
	WP_TYPE_9,
	WP_TYPE_10,
	WP_TYPE_11,
	WP_TYPE_END
};

typedef enum ENUM_STATUS_WP {
	STATUS_WP_NONE = 0,
	STATUS_WP_MOVE_FORWARD,
	STATUS_WP_STOP,
	STATUS_WP_ATTACK
};

typedef enum ENUM_TROOPS {
	TROOPS_NONE,
	TROOPS_BLUE,
	TROOPS_RED
};

typedef struct _S_PROPERTY_WP
{
	ENUM_WEAPON_TYPE WP_TYPE;
	int LEVEL;
	float SPEED;
	int RANGE_DEC;
	int DP;
	int HP;
	int RANGE_FIRE;
} S_PROPERTY_WP;

class Weapon
{
public:
	// 构造函数 与 析构函数
	Weapon();
	~Weapon();

	// 初始化函数
	void init(ENUM_TROOPS troops, S_PROPERTY_WP propertyWp, float posX, float posY);

protected:
	int mTroopsIn;

	// 武器型号
	S_PROPERTY_WP mPropertyWp;

	// 武器位置
	float mPosX;
	float mPosY;

	// 武器状态
	int mStatus;

public:
	// 存取函数
	void SetTroopsIn(int troopsIn){ mTroopsIn = troopsIn; };
	void SetProperty(S_PROPERTY_WP propertyWp){ memcpy(&mPropertyWp, &propertyWp, sizeof(S_PROPERTY_WP)); };
	void SetPosX(float posX){ mPosX = posX; };
	void SetPosY(float posY){ mPosY = posY; };
	void SetStatus(int status){ mStatus = status; };

	int GetTroopsIn(){ return mTroopsIn; };
	S_PROPERTY_WP GetProperty(){ return mPropertyWp; };
	float GetPosX(){ return mPosX; };
	float GetPosY(){ return mPosY; };
	int GetStatus(){ return mStatus; };

public:
	void Move();
	void Attack();
	int BeAttacked(int DP);
};