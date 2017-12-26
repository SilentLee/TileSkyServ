#pragma once
#include <vector>
#include "MultiThreadSync.h"
#include "CriticalSection.h"
#include "Weapon.h"
#include "BattleSituationMapCell.h"

using namespace std;

#define DEFALUT_GAME_TIME 30000

typedef enum ENUM_BATTLE_STATUS {
	BATTLE_STATUS_IN_PREPARING,
	BATTLE_STATUS_IN_PROGRESSING,
	BATTLE_STATUS_BLUE_WIN,
	BATTLE_STATUS_RED_WIN,
	BATTLE_STATUS_DRAW
};

class CBattleField : public CMultiThreadSync<CBattleField>
{
// ս��̬�Ʒ����ͼ
private:

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
	void SetStatus(ENUM_BATTLE_STATUS status) { CThreadSync Sync; mStatus = status; };
	ENUM_BATTLE_STATUS GetStatus() { CThreadSync Sync; return mStatus; };
	// ��ǰ��Ϸ��ʱ
	bool DecreaseRemainGameTime() { CThreadSync Sync; mRemainGameTime = max(0, mRemainGameTime--); return mRemainGameTime ? true : false; };
	int GetRemainGameTime() { CThreadSync Sync; return mRemainGameTime; };

	// ��ȡָ������
	Weapon* getTroopInBlueTeam(int index) { CThreadSync Sync; return &mTroopsInBlueTeam[index]; };
	Weapon* getTroopInRedTeam(int index) { CThreadSync Sync; return &mTroopsInRedTeam[index]; };
	// ��ȡ��������
	int getCountOfTroopsInBlueTeam() { CThreadSync Sync; return mTroopsInBlueTeam.size(); };
	int getCountOfTroopsInRedTeam() { CThreadSync Sync; return mTroopsInRedTeam.size(); };

	// ����ս����������ǩ������
	bool increaseWeaponTagCounter() { CThreadSync Sync; mWeaponTagCounter = mWeaponTagCounter++; };
	int getWeaponTagCounter() { CThreadSync Sync; return mWeaponTagCounter; };
};