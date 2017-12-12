#include "stdafx.h"
#include "Weapon.h"

// ���캯��
Weapon::Weapon()
{
	mTroopsIn = TROOPS_NONE;
	memset(&mPropertyWp, 0, sizeof(S_PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = WP_STATUS_NONE;
	mWeaponTag = 0;
}

// ��������
Weapon::~Weapon()
{
	mTroopsIn = TROOPS_NONE;
	memset(&mPropertyWp, 0, sizeof(S_PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = WP_STATUS_NONE;
	mWeaponTag = 0;
}

// ��ʼ������
void Weapon::init(ENUM_TROOPS troops, S_PROPERTY_WP propertyWp, float posX, float posY, int weaponTag)
{
	mTroopsIn = troops;
	memcpy(&mPropertyWp, &propertyWp, sizeof(S_PROPERTY_WP));
	mPosX = posX;
	mPosY = posY;

	// ����������ǩ
	mWeaponTag = weaponTag;

	// ������������״̬����Ϊ STATUS_WP_STOP 
	// �ɻ���������״̬����Ϊ STATUS_WP_MOVE_FORWARD
	if (propertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		mStatus = WP_STATUS_STOP;
	}
	else {
		mStatus = WP_STATUS_MOVING;
	}
}

// �����ƶ�
void Weapon::Move()
{
	// ����������Ч �� ����Ϊ������ �����ƶ� ��������
	if (mPropertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		return;
	}

	switch (mTroopsIn)
	{
	// ������������ ��ս���·������˶�
	case TROOPS_BLUE:
		this->mPosY += mPropertyWp.SPEED;
		break;

	// �췽�������� ��ս���Ϸ������˶�
	case TROOPS_RED:
		//mPosY = max(0.0f, mPosY - mPropertyWp.SPEED);
		mPosY = mPosY - mPropertyWp.SPEED;
		break;
	}
}

// ��������
void Weapon::Attack()
{

}

// �⵽����
int Weapon::BeAttacked(int DP)
{
	return mPropertyWp.HP = max(0, mPropertyWp.HP - DP);
}