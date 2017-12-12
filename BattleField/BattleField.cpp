#include "stdafx.h"
#include "BattleField.h"

// ս����ʼ������
void CBattleField::Begin()
{
	// �洢��ս˫�������Ķ�̬����
	mTroopsInBlueTeam.clear();
	mTroopsInRedTeam.clear();
	
	// ��ǰ��Ϸ״̬
	mStatus = BATTLE_STATUS_IN_PREPARING;
	// ʣ����Ϸʱ��
	mRemainGameTime = DEFALUT_GAME_TIME;
	// ������ǩ����������
	mWeaponTagCounter = 0;
}

// ս�����ú���
void CBattleField::End()
{
	// �洢��ս˫�������Ķ�̬����
	mTroopsInBlueTeam.clear();
	mTroopsInRedTeam.clear();

	// ��ǰ��Ϸ״̬
	mStatus = BATTLE_STATUS_IN_PREPARING;
	// ʣ����Ϸʱ��
	mRemainGameTime = DEFALUT_GAME_TIME;
	// ������ǩ����������
	mWeaponTagCounter = 0;
}

// ��ս��ʼ
void CBattleField::BattleStart()
{
	// �洢��ս˫�������Ķ�̬����
	mTroopsInBlueTeam.clear();
	mTroopsInRedTeam.clear();

	// ����ǰ��Ϸ״̬����Ϊ��Ϸ��
	mStatus = BATTLE_STATUS_IN_PROGRESSING;

	// ������Ϸʱ��
	mRemainGameTime = DEFALUT_GAME_TIME;
	// ������ǩ����������
	mWeaponTagCounter = 0;
}

Weapon CBattleField::InputTroops(ENUM_WEAPON_TYPE weaponType, ENUM_TROOPS troopsIn, float posX, float posY)
{
	printf("BattleField::inputTroopsBlue\n");

	// ����ʱʹ�õ��������Բ��� 
	// Ӧ������Ϊ�û������ݿ��ж�ȡ�Ĳ���
	ENUM_WEAPON_TYPE WP_TYPE = weaponType;
	int LEVEL = 1;
	float SPEED = 180;
	int RANGE_DEC = 5;
	int DP = 10;
	int HP = 100;
	int RANGE_FIRE = 5;

	S_PROPERTY_WP propertyWp = S_PROPERTY_WP{ WP_TYPE, LEVEL, SPEED, RANGE_DEC, DP, HP, RANGE_FIRE };

	Weapon Weapon;

	// ������ս���еı�ǩ
	int WeaponTag = mWeaponTagCounter;
	// ���¼���������ս���е�������
	if (troopsIn == TROOPS_BLUE) {
		// �������·����Ϸ���
		posY = posY;
	}
	else if (troopsIn == TROOPS_RED) {
		// �췽���Ϸ����·���
		posY = 2060 - posY;
	}

	Weapon.init(troopsIn, propertyWp, posX, posY, WeaponTag);
	mWeaponTagCounter++;

	// �����������Ӧ���������ߺ췽
	if (troopsIn == TROOPS_BLUE) {
		mTroopsInBlueTeam.push_back(Weapon);
	}
	else if (troopsIn == TROOPS_RED) {
		mTroopsInRedTeam.push_back(Weapon);
	}

	return Weapon;
}

// ս��̬��ˢ��
ENUM_BATTLE_STATUS CBattleField::UpdateBattleFieldSituation()
{
	printf("BattleField::updateBattleFieldSituation time = %d\n", mRemainGameTime);

	// ˢ����������λ��
	for (int index = 0; index < mTroopsInBlueTeam.size(); index++) {
		mTroopsInBlueTeam[index].Move();
	}

	// ˢ�º췽����λ��
	for (int index = 0; index < mTroopsInRedTeam.size(); index++) {
		mTroopsInRedTeam[index].Move();
	}

	// ��Ϸ��ʱ�� 1
	DecreaseRemainGameTime();

	// ����ʱ�� 0 ʱ ��Ϸ����
	if (mRemainGameTime == 0) {
		mStatus = BATTLE_STATUS_DRAW;
	}
	// ��ʱ��δ����
	else {
		mStatus = BATTLE_STATUS_IN_PROGRESSING;
	}

	return mStatus;
}