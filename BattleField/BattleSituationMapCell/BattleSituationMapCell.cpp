#include "stdafx.h"
#include "BattleSituationMapCell.h"

void CBattleSituationMapCell::init(Vec2 coordinate)
{
	// ����ս������ķ�������
	mCoordinate = coordinate;

	// ����λ�ڷ����������������
	mWeaponType = WP_TYPE_NONE;
	// ����λ�ڷ��������������ǩ
	mWeaponTag = TAG_NO_WEAPON_IN;

	// ���ö�ս˫���ֱ���Ե�ս������
	mFogAgainstBlueEn = true;
	mFogAgainstRedEn = true;

	// ��յ�Ų����䶯̬����
	mHertzWaveVec.clear();
}