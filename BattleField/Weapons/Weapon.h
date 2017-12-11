#pragma once
#include "string.h"
#include <algorithm>

using namespace std;

typedef enum ENUM_WEAPON_TYPE {
	WP_TYPE_NONE = 0,

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
	// ���캯�� �� ��������
	Weapon();
	~Weapon();

	// ��ʼ������
	void init(ENUM_TROOPS troops, S_PROPERTY_WP propertyWp, float posX, float posY);

protected:
	int mTroopsIn;

	// �����ͺ�
	S_PROPERTY_WP mPropertyWp;

	// ����λ��
	float mPosX;
	float mPosY;

	// ����״̬
	int mStatus;

public:
	// ��ȡ����
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