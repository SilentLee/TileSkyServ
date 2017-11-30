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
}

// Ͷ��ս������
void CBattleField::InputTroops(int weaponType, int sideInGame, float posX, float posY)
{
	printf("BattleField::inputTroops\n");

	// ����ʱʹ�õ��������Բ��� 
	// Ӧ������Ϊ�û������ݿ��ж�ȡ�Ĳ���
	int WP_TYPE = weaponType;
	int LEVEL = 1;
	float SPEED = 10;
	int RANGE_DEC = 5;
	int DP = 10;
	int HP = 100;
	int RANGE_FIRE = 5;

	PROPERTY_WP propertyWp = PROPERTY_WP{ WP_TYPE, LEVEL, SPEED, RANGE_DEC, DP, HP, RANGE_FIRE };

	Weapon weapon;
	weapon.init(sideInGame, propertyWp, posX, posY);

	if (sideInGame == SIDE_BLUE) {
		mTroopsInBlueTeam.push_back(weapon);
	}
	else {
		mTroopsInRedTeam.push_back(weapon);
	}
}

// ս��̬��ˢ��
BATTLE_STATUS CBattleField::UpdateBattleFieldSituation()
{
	printf("BattleField::updateBattleFieldSituation\n");

	// ˢ����������λ��
	for (int index = 0; index < mTroopsInBlueTeam.size(); index++) {
		mTroopsInBlueTeam[index].move();
	}

	// ˢ�º췽����λ��
	for (int index = 0; index < mTroopsInRedTeam.size(); index++) {
		mTroopsInRedTeam[index].move();
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