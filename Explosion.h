#pragma once

typedef struct _S_EXPLOSION
{
    INT EXPLOSION_NO;
    INT WIDTH;
    INT EXPLOSION_TYPE;
    INT POS_X;
    INT POS_Y;
    INT IS_DISABLE;
    INT DURATION;
} S_EXPLOSION;

class CExplosion
{
private:
    S_EXPLOSION mExplosion;
public:
    CExplosion(int ExplosionNo, int width, int ExplosionType, int x, int y);
    ~CExplosion();
    
public:
    void setExplosionNo(int explosionNo){mExplosion.EXPLOSION_NO = explosionNo;}
    int getExplosionNo(){return mExplosion.EXPLOSION_NO;}
    
    void setWidth(int width){mExplosion.WIDTH = width;}
    int getWidth(){return mExplosion.WIDTH;}
    
    void setExplosionType(int explosionType){mExplosion.EXPLOSION_TYPE = explosionType;}
    int getExplosionType(){return mExplosion.EXPLOSION_TYPE;}
    
    void setPosX(int x){mExplosion.POS_X = x;}
    int getPosX(){return mExplosion.POS_X;}
    
    void setPosY(int y){mExplosion.POS_Y = y;}
    int getPosY(){return mExplosion.POS_Y;}
    
    void setIsDisable(int isDisable){mExplosion.IS_DISABLE = isDisable;}
    int getIsDisable(){return mExplosion.IS_DISABLE;}
    
    void setDuration(int duration){mExplosion.DURATION = duration;}
    int getDuration(){return mExplosion.DURATION;}
	int decreaseDuration(){return mExplosion.DURATION = max(mExplosion.DURATION --, -1);}
};
