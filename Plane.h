#pragma once

typedef struct _S_PLANE
{
	INT PLANE_NO;
	INT PLANE_TYPE;
	INT ALIVE_ENGINE_NUM;
	INT POS_X;
	INT POS_Y;
	INT ROTATION;
	INT IS_SHOT_DOWN;
	INT FALLING_DURATION;
} S_PLANE;

class CPlane
{
private:
	S_PLANE mPlane;

	//int mPlaneNo;
	//int mPlaneType;
	//int mAliveEngineNum;
	//int mPosX;
	//int mPosY;
	//int mRotation;

	//bool mIsShotDown;

public:
	CPlane(int planeNo, int planeType, int aliveEngineNum, int posX, int posY, int rotation);
	~CPlane();
// 击毁引擎位置后的处理
public:
	int DescreaseAliveEngineNum(){mPlane.ALIVE_ENGINE_NUM = max(0, mPlane.ALIVE_ENGINE_NUM--); return mPlane.ALIVE_ENGINE_NUM;}
	//int DescreaseAliveEngineNum(){mAliveEngineNum = max(0, mAliveEngineNum--); return mAliveEngineNum;}
// 存取函数
public:
	void setPlane(S_PLANE plane){memcpy(&mPlane, &plane, sizeof(S_PLANE));}
	S_PLANE* getPlane(){return &mPlane;}

	void setPlaneNo(int planeNo){mPlane.PLANE_NO = planeNo;}
	int getPlaneNo(){return mPlane.PLANE_NO;}

	void setPlaneType(int planeType){mPlane.PLANE_TYPE = planeType;}
	int getPlaneType(){return mPlane.PLANE_TYPE ;}

	void setAliveEngineNum(int aliveEngineNum){mPlane.ALIVE_ENGINE_NUM = aliveEngineNum;}
	int getAliveEngineNum(){return mPlane.ALIVE_ENGINE_NUM;}

	void setPosX(int posX){mPlane.POS_X = posX;}
	int getPosX (){return mPlane.POS_X;}

	void setPosY(int posY){mPlane.POS_Y = posY;}
	int getPosY(){return mPlane.POS_Y;}

	void setRotation(int rotation){mPlane.ROTATION = rotation;}
	int getRotation(){return mPlane.ROTATION;}

	void setIsShotDown(bool isShotDown){mPlane.IS_SHOT_DOWN = isShotDown;}
	int getIsShotDown(){return mPlane.IS_SHOT_DOWN;}

	void setFallingDuration(int fallingDuration){mPlane.FALLING_DURATION = fallingDuration;}
	int getFallingDuration(){return mPlane.FALLING_DURATION;}
	int decreaseFallingDuration(){mPlane.FALLING_DURATION = max(mPlane.FALLING_DURATION--, -1); return mPlane.FALLING_DURATION;}

	// 飞机被击落时的处理函数
	bool onPlaneShotDown(void* battleField);
	bool onJuniorPlaneShotDown(void* battleField);
	bool onMiddlePlaneShotDown(void* battleField);
	bool onSeniorPlaneShotDown(void* battleField);
	bool onJuniorStealthPlaneShotDown(void* battleField);
	bool onMiddleStealthPlaneShotDown(void* battleField);
	bool onSeniorStealthPlaneShotDown(void* battleField);
};