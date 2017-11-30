#include "stdafx.h"
#include <stdio.h>
#include "Radar.h"
#include "GameCtrlParams.h"

CRadar::CRadar(int radarNo, int width, int radarType, int x, int y)
{
    mRadar.RADAR_NO = radarNo;
    mRadar.WIDTH = width;
    mRadar.RADAR_TYPE = radarType;
    mRadar.POS_X = x;
    mRadar.POS_Y = y;
    mRadar.IS_DISABLE = false;
    
    switch(radarType)
    {
        case JUNIOR_RADAR:
            mRadar.DURATION = BASIC_DURATING_TIME_OF_JUNIOR_RADAR;
            break;
            
        case MIDDLE_RADAR:
            mRadar.DURATION = BASIC_DURATING_TIME_OF_MIDDLE_RADAR;
            break;
            
        case SENIOR_RADAR:
            mRadar.DURATION = BASIC_DURATING_TIME_OF_SENIOR_RADAR;
            break;
            
        case JUNIOR_ANTI_STEALTH_RADAR:
            mRadar.DURATION = BASIC_DURATING_TIME_OF_JUNIOR_ANTI_STEALTH_RADAR;
            break;
            
        case MIDDLE_ANTI_STEALTH_RADAR:
            mRadar.DURATION = BASIC_DURATING_TIME_OF_MIDDLE_ANTI_STEALTH_RADAR;
            break;
            
        case SENIOR_ANTI_STEALTH_RADAR:
            mRadar.DURATION = BASIC_DURATING_TIME_OF_SENIOR_ANTI_STEALTH_RADAR;
            break;
            
        default:
            break;
    }
}

CRadar::~CRadar()
{
    
}













































