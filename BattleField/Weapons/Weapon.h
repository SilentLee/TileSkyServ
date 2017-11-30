#pragma once
#include "string.h"
#include <algorithm>

using namespace std;

typedef enum WEAPON_TYPE {
	NO_WP_TYPE = 0,

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

typedef enum STATUS_WP {
	STATUS_WP_NONE = 0,
	STATUS_WP_MOVE_FORWARD,
	STATUS_WP_STOP,
	STATUS_WP_ATTACK
};

typedef enum SIDE_IN_GAME {
	SIDE_NONE,
	SIDE_BLUE,
	SIDE_RED
};

typedef struct _PROPERTY_WP
{
	int WP_TYPE;
	int LEVEL;
	float SPEED;
	int RANGE_DEC;
	int DP;
	int HP;
	int RANGE_FIRE;
} PROPERTY_WP;

class Weapon
{
public:
	// 构造函数 与 析构函数
	Weapon();
	~Weapon();

	// 初始化函数
	void init(int sideInGame, PROPERTY_WP propertyWp, float posX, float posY);

protected:
	int mSideInGame;

	// 武器型号
	PROPERTY_WP mPropertyWp;

	// 武器位置
	float mPosX;
	float mPosY;

	// 武器状态
	int mStatus;

public:
	// 存取函数
	void setSideInGame(int sideInGame);
	void setProperty(PROPERTY_WP propertyWp);
	void setPosX(float posX);
	void setPosY(float posY);
	void setStatus(int status);

	int getSideInGame();
	PROPERTY_WP getProperty();
	float getPosX();
	float getPosY();
	int getStatus();

public:
	void move();
	void attack();
	int beAttacked(int DP);
};