#pragma once
#include "string.h"
#include <algorithm>

using namespace std;

typedef enum WEAPON_TYPE {
	NO_WP_TYPE = 0,

	/************����Ϊ������������ �����ƶ�*************/
	WP_TYPE_ANTI_AIRCRAFT_MISSILE,
	WP_TYPE_FLAK, // ������
	WP_TYPE_RADAR,

	/************����Ϊ�ɻ��������� �����ƶ�*************/
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
	// ���캯�� �� ��������
	Weapon();
	~Weapon();

	// ��ʼ������
	void init(int sideInGame, PROPERTY_WP propertyWp, float posX, float posY);

protected:
	int mSideInGame;

	// �����ͺ�
	PROPERTY_WP mPropertyWp;

	// ����λ��
	float mPosX;
	float mPosY;

	// ����״̬
	int mStatus;

public:
	// ��ȡ����
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