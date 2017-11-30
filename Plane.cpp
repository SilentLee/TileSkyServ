#include "stdafx.h"
#include "Plane.h"
#include "GameCtrlParams.h"
#include "BattleField.h"

CPlane::CPlane(int planeNo, int planeType, int aliveEngineNum, int posX, int posY, int rotation)
{
	mPlane.PLANE_NO = planeNo;
	mPlane.PLANE_TYPE = planeType;
	mPlane.ALIVE_ENGINE_NUM = aliveEngineNum;
	mPlane.POS_X = posX;
	mPlane.POS_Y = posY;
	mPlane.ROTATION = rotation;
	mPlane.IS_SHOT_DOWN = false;
	
	switch(planeType)
	{
	case JUNIOR_PLANE:
		mPlane.FALLING_DURATION = FALLING_DURATION_OF_JUNIOR_PLANE;
		break;

	case MIDDLE_PLANE:
		mPlane.FALLING_DURATION = FALLING_DURATION_OF_MIDDLE_PLANE;
		break;

	case SENIOR_PLANE:
		mPlane.FALLING_DURATION = FALLING_DURATION_OF_SENIOR_PLANE;
		break;

	case JUNIOR_STEALTH_PLANE:
		mPlane.FALLING_DURATION = FALLING_DURATION_OF_JUNIOR_STEALTH_PLANE;
		break;

	case MIDDLE_STEALTH_PLANE:
		mPlane.FALLING_DURATION = FALLING_DURATION_OF_MIDDLE_STEALTH_PLANE;
		break;

	case SENIOR_STEALTH_PLANE:
		mPlane.FALLING_DURATION = FALLING_DURATION_OF_SENIOR_STEALTH_PLANE;
		break;

	default:
		break;
	}
}

CPlane::~CPlane()
{

}

bool CPlane::onPlaneShotDown(void* battleField)
{
	switch(mPlane.PLANE_TYPE)
	{
	case JUNIOR_PLANE:
		return onJuniorPlaneShotDown(battleField);
		break;

	case MIDDLE_PLANE:
		return onMiddlePlaneShotDown(battleField);
		break;

	case SENIOR_PLANE:
		return onSeniorPlaneShotDown(battleField);
		break;
	case JUNIOR_STEALTH_PLANE:
		return onJuniorStealthPlaneShotDown(battleField);
		break;

	case MIDDLE_STEALTH_PLANE:
		return onMiddleStealthPlaneShotDown(battleField);
		break;

	case SENIOR_STEALTH_PLANE:
		return onSeniorStealthPlaneShotDown(battleField);
		break;
	}
	return true;
}

bool CPlane::onJuniorPlaneShotDown(void* battleField)
{
	CBattleField* BattleField = (CBattleField*)battleField;
	vector<CBattleFieldCell*> battleFieldCellVec = BattleField->getBattleCellVec();
	int width = BattleField->getSize();

	switch(mPlane.ROTATION)
    {
        case 0:
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
			battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 90:
			battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 180:
			battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            break;
            
        case 270:
			battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            break;
    }
	mPlane.IS_SHOT_DOWN = true;
	return true;
}

bool CPlane::onMiddlePlaneShotDown(void* battleField)
{
	CBattleField* BattleField = (CBattleField*)battleField;
	vector<CBattleFieldCell*> battleFieldCellVec = BattleField->getBattleCellVec();
	int width = BattleField->getSize();

	 switch(mPlane.ROTATION)
    {
        case 0:
			battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 90:
			battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+4) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-4) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 180:
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 270:
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+4) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-0) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-4) * width)->setStatus(STATUS_DESTROYED);
            break;
	}
	mPlane.IS_SHOT_DOWN = true;
	return true;
}

bool CPlane::onSeniorPlaneShotDown(void* battleField)
{
	CBattleField* BattleField = (CBattleField*)battleField;
	vector<CBattleFieldCell*> battleFieldCellVec = BattleField->getBattleCellVec();
	int width = BattleField->getSize();

	switch(mPlane.ROTATION)
    {
        case 0:
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-6) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-5) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+5) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+6) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 90:
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-6) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-5) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-3) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+3) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+5) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+6) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            break;
            
        case 180:
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-6) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-5) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+5) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+6) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 270:
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-6) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-5) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-3) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+3) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+5) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+6) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            break;
    }
	mPlane.IS_SHOT_DOWN = true;
	return true;
}

bool CPlane::onJuniorStealthPlaneShotDown(void* battleField)
{
	CBattleField* BattleField = (CBattleField*)battleField;
	vector<CBattleFieldCell*> battleFieldCellVec = BattleField->getBattleCellVec();
	int width = BattleField->getSize();

	switch(mPlane.ROTATION)
    {
        case 0:
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
			battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 90:
			battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 180:
			battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            break;
            
        case 270:
			battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            break;
    }
	mPlane.IS_SHOT_DOWN = true;
	return true;
}

bool CPlane::onMiddleStealthPlaneShotDown(void* battleField)
{
	CBattleField* BattleField = (CBattleField*)battleField;
	vector<CBattleFieldCell*> battleFieldCellVec = BattleField->getBattleCellVec();
	int width = BattleField->getSize();

	 switch(mPlane.ROTATION)
    {
        case 0:
			battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 90:
			battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+4) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-4) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 180:
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 270:
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+4) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-0) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-4) * width)->setStatus(STATUS_DESTROYED);
            break;
	}
	mPlane.IS_SHOT_DOWN = true;
	return true;
}

bool CPlane::onSeniorStealthPlaneShotDown(void* battleField)
{
	CBattleField* BattleField = (CBattleField*)battleField;
	vector<CBattleFieldCell*> battleFieldCellVec = BattleField->getBattleCellVec();
	int width = BattleField->getSize();

	switch(mPlane.ROTATION)
    {
        case 0:
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-6) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-5) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+5) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+6) + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 90:
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-6) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-5) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-3) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+3) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+5) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+6) * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            break;
            
        case 180:
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y-2) * width)->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_X-6) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-5) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+3) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+5) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+6) + (mPlane.POS_Y-1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+4) + mPlane.POS_Y * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+1) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+2) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-2) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X-1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_X + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+1) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_X+2) + (mPlane.POS_Y+3) * width)->setStatus(STATUS_DESTROYED);
            break;
            
        case 270:
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X-2))->setStatus(STATUS_DESTROYED); // 引擎位置
            battleFieldCellVec.at((mPlane.POS_Y-6) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-5) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-3) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+3) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+5) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+6) * width + (mPlane.POS_X-1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+4) * width + mPlane.POS_X)->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+1))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+2))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-2) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y-1) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at(mPlane.POS_Y * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+1) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            battleFieldCellVec.at((mPlane.POS_Y+2) * width + (mPlane.POS_X+3))->setStatus(STATUS_DESTROYED);
            break;
    }
	mPlane.IS_SHOT_DOWN = true;
	return true;
}