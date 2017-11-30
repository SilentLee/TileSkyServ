#include "stdafx.h"
#include "Weapon.h"

// ���캯��
Weapon::Weapon()
{
	mSideInGame = SIDE_NONE;
	memset(&mPropertyWp, 0, sizeof(PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = STATUS_WP_NONE;
}

// ��������
Weapon::~Weapon()
{
	mSideInGame = SIDE_NONE;
	memset(&mPropertyWp, 0, sizeof(PROPERTY_WP));
	mPosX = 0;
	mPosY = 0;
	mStatus = STATUS_WP_NONE;
}

// ��ʼ������
void Weapon::init(int sideInGame, PROPERTY_WP propertyWp, float posX, float posY)
{
	mSideInGame = sideInGame;
	memcpy(&mPropertyWp, &propertyWp, sizeof(PROPERTY_WP));
	mPosX = posX;
	mPosY = posY;

	// ������������״̬����Ϊ STATUS_WP_STOP 
	// �ɻ���������״̬����Ϊ STATUS_WP_MOVE_FORWARD
	if (propertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		mStatus = STATUS_WP_STOP;
	}
	else {
		mStatus = STATUS_WP_MOVE_FORWARD;
	}
}

// ��ȡ����
// ��ȡ
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

// д��
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

// �����ƶ�
void Weapon::move()
{
	// ����������Ч �� ����Ϊ������ �����ƶ� ��������
	if (mPropertyWp.WP_TYPE <= WP_TYPE_RADAR) {
		return;
	}

	switch (mSideInGame)
	{
	// ������������ ��ս���·������˶�
	case SIDE_BLUE:
		this->mPosY += mPropertyWp.SPEED;
		break;

	// �췽�������� ��ս���Ϸ������˶�
	case SIDE_RED:
		mPosY = max(0.0f, mPosY - mPropertyWp.SPEED);
		break;
	}
}

// ��������
void Weapon::attack()
{

}

// �⵽����
int Weapon::beAttacked(int DP)
{
	return mPropertyWp.HP = max(0, mPropertyWp.HP - DP);
}