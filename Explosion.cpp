#include "stdafx.h"
#include <stdio.h>
#include "Explosion.h"
#include "GameCtrlParams.h"

CExplosion::CExplosion(int explosionNo, int width, int explosionType, int x, int y)
{
    mExplosion.EXPLOSION_NO = explosionNo;
    mExplosion.WIDTH = width;
    mExplosion.EXPLOSION_TYPE = explosionType;
    mExplosion.POS_X = x;
    mExplosion.POS_Y = y;
    mExplosion.IS_DISABLE = false;
    
    switch(explosionType)
    {
        case JUNIOR_CANNONBALL:
            mExplosion.DURATION = JUNIOR_EXPLOSION_DURATION;
            break;
            
        case MIDDLE_CANNONBALL:
            mExplosion.DURATION = MIDDLE_EXPLOSION_DURATION;
            break;
            
        case SENIOR_CANNONBALL:
            mExplosion.DURATION = SENIOR_EXPLOSION_DURATION;
            break;
            
        case JUNIOR_MISSILE:
            mExplosion.DURATION = JUNIOR_EXPLOSION_DURATION;
            break;
            
        case MIDDLE_MISSILE:
            mExplosion.DURATION = MIDDLE_EXPLOSION_DURATION;
            break;
            
        case SENIOR_MISSILE:
            mExplosion.DURATION = SENIOR_EXPLOSION_DURATION;
            break;
            
        default:
            break;
    }
}

CExplosion::~CExplosion()
{
    
}





























