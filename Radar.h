#pragma once

typedef struct _S_RADAR
{
    INT RADAR_NO;
    INT WIDTH;
    INT RADAR_TYPE;
    INT POS_X;
    INT POS_Y;
    INT IS_DISABLE;
    INT DURATION;
} S_RADAR;

class CRadar
{
private:
    S_RADAR mRadar;
public:
    CRadar(int radarNo, int width, int radarType, int x, int y);
    ~CRadar();
    
public:
    void setRadarNo(int radarNo){mRadar.RADAR_NO = radarNo;}
    int getRadarNo(){return mRadar.RADAR_NO;}
    
    void setWidth(int width){mRadar.WIDTH = width;}
    int getWidth(){return mRadar.WIDTH;}
    
    void setRadarType(int radarType){mRadar.RADAR_TYPE = radarType;}
    int getRadarType(){return mRadar.RADAR_TYPE;}
    
    void setPosX(int x){mRadar.POS_X = x;}
    int getPosX(){return mRadar.POS_X;}
    
    void setPosY(int y){mRadar.POS_Y = y;}
    int getPosY(){return mRadar.POS_Y;}
    
    void setIsDisable(int isDisable){mRadar.IS_DISABLE = isDisable;}
    int getIsDisable(){return mRadar.IS_DISABLE;}
    
    void setDuration(int duration){mRadar.DURATION = duration;}
    int getDuration(){return mRadar.DURATION;}
	int decreaseDuration(){return  mRadar.DURATION = max(mRadar.DURATION --, -1);}
};


















