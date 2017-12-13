#pragma once
#include "MultiThreadSync.h"
#include "CriticalSection.h"
#include "string.h"
#include <algorithm>

using namespace std;

// ��������
enum ENUM_WEAPON_TYPE {
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

	/********����Ϊ������������ �����ƶ� ���ܲ���********/
	WP_TYPE_AAM,

	WP_TYPE_END
};

typedef enum ENUM_WEAPON_STATUS {
	WP_STATUS_NONE,
	WP_STATUS_STOP,
	WP_STATUS_MOVING,
	WP_STATUS_ATTACKING,
	WP_STATUS_DESTROIED
};

typedef enum ENUM_MOVE_DIRECTION {
	ENUM_MOVE_DIRECTION_NONE = -1,
	ENUM_MOVE_DIRECTION_0 = 0,
	ENUM_MOVE_DIRECTION_45 = 45,
	ENUM_MOVE_DIRECTION_90 = 90,
	ENUM_MOVE_DIRECTION_135 = 135,
	ENUM_MOVE_DIRECTION_180 = 180,
	ENUM_MOVE_DIRECTION_225 = 225,
	ENUM_MOVE_DIRECTION_270 = 270,
	ENUM_MOVE_DIRECTION_315 = 315,
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

class Weapon : public CMultiThreadSync<Weapon>
{
public:
	// ���캯�� �� ��������
	Weapon();
	~Weapon();

	// ��ʼ������
	void init(ENUM_TROOPS troops, S_PROPERTY_WP propertyWp, float posX, float posY, int weaponTag);

protected:
	ENUM_TROOPS mTroopsIn;

	// �����ͺ�
	S_PROPERTY_WP mPropertyWp;

	// ����λ��
	float mPosX;
	float mPosY;

	// ����״̬
	int mStatus;

	// ������ս���еı�ǩ���
	int mWeaponTag;

public:
	// ��ȡ����
	void SetTroopsIn(ENUM_TROOPS troopsIn) { CThreadSync Sync; mTroopsIn = troopsIn; };
	void SetProperty(S_PROPERTY_WP propertyWp) { CThreadSync Sync; memcpy(&mPropertyWp, &propertyWp, sizeof(S_PROPERTY_WP)); };
	void SetPosX(float posX) { CThreadSync Sync; mPosX = posX; };
	void SetPosY(float posY) { CThreadSync Sync; mPosY = posY; };
	void SetStatus(int status) { CThreadSync Sync; mStatus = status; };
	void SetWeaponTag(int weaponTag) { CThreadSync Sync; mWeaponTag = weaponTag; };

	ENUM_TROOPS GetTroopsIn() { CThreadSync Sync; return mTroopsIn; };
	S_PROPERTY_WP GetProperty() { CThreadSync Sync; return mPropertyWp; };
	float GetPosX() { CThreadSync Sync; return mPosX; };
	float GetPosY() { CThreadSync Sync; return mPosY; };
	int GetStatus() { CThreadSync Sync; return mStatus; };
	int GetWeaponTag() { CThreadSync Sync; return mWeaponTag; };

public:
	void Move();
	void Attack();
	int BeAttacked(int DP);
};