#pragma once
#include "Weapon.h"
#include <vector>

// ս��ģ���ͼ������
#define WIDTH_OF_BATTLE_SIMULATION_MAP_CELL			27.0f
// ս��̬�Ʒ����ͼ���
#define WIDTH_OF_BATTLE_SIMULATION_MAP							40
// ս��̬�Ʒ����ͼ�߶�
#define HEIGHT_OF_BATTLE_SIMULATION_MAP						60
// ս��̬����ʾ��ͼ���
#define WIDTH_OF_BATTLE_DISPLAY_MAP								1080.0f
// ս��̬����ʾ��ͼ�߶�
#define HEIGHT_OF_BATTLE_DISPLAY_MAP								1620.0f

// ������ǩ��ʼֵ��
#define TAG_NO_WEAPON_IN															-1

typedef struct _S_HERTZ_WAVE {
	// ��ƽ
	float LEVEL;
	// ����ƽ̨������ǩ
	int TAG;
	// ���䷽��
	int DIRECTION_FROM;
} S_HERTZ_WAVE;

typedef struct _Vec2 {
	float x;
	float y;
} Vec2;

class CBattleSituationMapCell
{
private:
	// ��ͼ��������
	Vec2 mCoordinate;
	// ���ڵ�ǰ����ռ��е���������
	ENUM_WEAPON_TYPE mWeaponType;
	// ���ڵ�ǰ����ռ��е�������ǩ
	int mWeaponTag;
	// ������Ե�ս������ʹ��
	bool mFogAgainstBlueEn;
	// �췽��Ե�ս������ʹ��
	bool mFogAgainstRedEn;
	// ����ڵ�ǰ����ռ��еĵ�Ų�����
	vector<S_HERTZ_WAVE> mHertzWaveVec;

public:
	void init(Vec2 coordinate);

// ��ȡ����
public:
	void SetCoordinate(Vec2 coordinate) { mCoordinate = coordinate; };
	Vec2 GetCoordinate() { return mCoordinate; };

	void SetWeaponType(ENUM_WEAPON_TYPE weaponType) { mWeaponType = weaponType; };
	ENUM_WEAPON_TYPE GetWeaponTYpe() { return mWeaponType; };

	void SetWeaponTag(int weaponTag) { mWeaponTag = weaponTag; };
	int GetWeaponTag() { return mWeaponTag; };
	void resetWeaponTag() { mWeaponTag = TAG_NO_WEAPON_IN; };

	void SetFogAgainstBlueEn(bool fogAgainstBlueEn) { mFogAgainstBlueEn = fogAgainstBlueEn; };
	int GetFogAgainstBlueEn() { return mFogAgainstBlueEn; };

	void SegFogAgainstRedEn(bool fogAgainstRedEn) { mFogAgainstRedEn = fogAgainstRedEn; };
	int GetFogAgainstRedEn() { return mFogAgainstRedEn; };
};