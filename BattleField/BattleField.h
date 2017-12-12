#pragma once
#include <vector>
#include "Weapon.h"

using namespace std;

#define DEFALUT_GAME_TIME 30000

typedef enum ENUM_BATTLE_STATUS {
	BATTLE_STATUS_IN_PREPARING,
	BATTLE_STATUS_IN_PROGRESSING,
	BATTLE_STATUS_BLUE_WIN,
	BATTLE_STATUS_RED_WIN,
	BATTLE_STATUS_DRAW
};

class CBattleField
{
private:
	// �洢��ս˫�������Ķ�̬����
	vector<Weapon> mTroopsInBlueTeam;
	vector<Weapon> mTroopsInRedTeam;

	// ��ǰ��Ϸ״̬
	ENUM_BATTLE_STATUS mStatus;
	// ʣ����Ϸʱ��
	int mRemainGameTime;

public:
	// ս����ʼ������
	void Begin();
	// ս�����ú���
	void End();
	// ��ս��ʼ
	void BattleStart();
public:
	// ����Ͷ��ս�� ���ؼ���ս��������
	Weapon InputTroops(ENUM_WEAPON_TYPE weaponType, ENUM_TROOPS troopsIn, float posX, float posY);
	// ˢ��ս��̬�� ͬʱ������Ϸ״̬
	ENUM_BATTLE_STATUS UpdateBattleFieldSituation();
	// ս����������ǩ������
	int mWeaponTagCounter;

// ��ȡ����
public:
	// ��ǰս��״̬
	void SetStatus(ENUM_BATTLE_STATUS status) { mStatus = status; };
	ENUM_BATTLE_STATUS GetStatus() { return mStatus; };
	// ��ǰ��Ϸ��ʱ
	bool DecreaseRemainGameTime() { mRemainGameTime = max(0, mRemainGameTime--); return mRemainGameTime ? true : false; };
	int GetRemainGameTime() { return mRemainGameTime; };

	// ��ȡָ������
	Weapon* getTroopInBlueTeam(int index) { return &mTroopsInBlueTeam[index]; };
	Weapon* getTroopInRedTeam(int index) { return &mTroopsInRedTeam[index]; };
	// ��ȡ��������
	int getCountOfTroopsInBlueTeam() { return mTroopsInBlueTeam.size(); };
	int getCountOfTroopsInRedTeam() { return mTroopsInRedTeam.size(); };

	// ����ս����������ǩ������
	bool increaseWeaponTagCounter(){ mWeaponTagCounter = mWeaponTagCounter++; };
	int getWeaponTagCounter(){ return mWeaponTagCounter; };
};