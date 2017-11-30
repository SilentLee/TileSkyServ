#include "stdafx.h"
#include "ConnectedUser.h"
#include "GameCtrlParams.h"

S_PT_USER_NAME_LOGIN_SUCC_U* CConnectedUser::db_UserNameLogin(CMySqlSessionPool mySqlSessionPool, S_PT_USER_NAME_LOGIN ptUserNameLogin)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();
	S_PT_USER_NAME_LOGIN_SUCC_U *ptUserNameLoginSuccU = new S_PT_USER_NAME_LOGIN_SUCC_U();
	memset(ptUserNameLoginSuccU, 0, sizeof(S_PT_USER_NAME_LOGIN_SUCC_U));

	// 查询用户账户信息
	sprintf(mySqlSession->mCommand, "select * from %s where %s = '%s' and %s = '%s'", 
		USER_TABLE_NAME, 
		"USER_NAME", 
		ptUserNameLogin.USER_NAME,  
		"PASSWORD", 
		ptUserNameLogin.PASSWORD);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("USER_ID", &ptUserNameLoginSuccU->USER_ID);
	mySqlResReader->GetCHARs("NICKNAME", ptUserNameLoginSuccU->NICKNAME);
	mySqlResReader->GetCHARs("PASSWORD", ptUserNameLoginSuccU->PASSWORD);
	mySqlResReader->GetCHARs("BIND_ACCOUNT_TYPE", ptUserNameLoginSuccU->BIND_ACCOUNT_TYPE);
	mySqlResReader->GetCHARs("BIND_ACCOUNT", ptUserNameLoginSuccU->BIND_ACCOUNT);
	mySqlResReader->GetCHARs("CELLPHONE_NO", ptUserNameLoginSuccU->CELLPHONE_NO);
	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptUserNameLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptUserNameLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptUserNameLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptUserNameLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptUserNameLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptUserNameLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptUserNameLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptUserNameLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptUserNameLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptUserNameLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptUserNameLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptUserNameLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptUserNameLoginSuccU->SESSION_ID);

	//delete mySqlResReader;

	// 查询用户装备信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		EQUIPMENT_TABLE_NAME,
		ptUserNameLoginSuccU->USER_ID,
		ptUserNameLoginSuccU->PASSWORD);
	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("JUNIOR_PLANE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_JUNIOR_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_PLANE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_MIDDLE_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_PLANE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_SENIOR_PLANE_CARD);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_JUNIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_MIDDLE_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_SENIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_SENIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_MIDDLE_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_JUNIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_MISSILE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_SENIOR_MISSILE_CARD);
	mySqlResReader->GetInt("MIDDLE_MISSILE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_MIDDLE_MISSILE_CARD);
	mySqlResReader->GetInt("JUNIOR_MISSILE_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_JUNIOR_MISSILE_CARD);
	mySqlResReader->GetInt("SENIOR_RADAR_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_SENIOR_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_RADAR_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_MIDDLE_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_RADAR_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_JUNIOR_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_SENIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_MIDDLE_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_CARD_NUM", &ptUserNameLoginSuccU->NUM_OF_JUNIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_PLANE_LEVEL", &ptUserNameLoginSuccU->LEVEL_JUNIOR_PLANE);
	mySqlResReader->GetInt("MIDDLE_PLANE_LEVEL", &ptUserNameLoginSuccU->LEVEL_MIDDLE_PLANE);
	mySqlResReader->GetInt("SENIOR_PLANE_LEVEL", &ptUserNameLoginSuccU->LEVEL_SENIOR_PLANE);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_LEVEL", &ptUserNameLoginSuccU->LEVEL_JUNIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_LEVEL", &ptUserNameLoginSuccU->LEVEL_MIDDLE_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_LEVEL", &ptUserNameLoginSuccU->LEVEL_SENIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_LEVEL", &ptUserNameLoginSuccU->LEVEL_SENIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_LEVEL", &ptUserNameLoginSuccU->LEVEL_MIDDLE_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_LEVEL", &ptUserNameLoginSuccU->LEVEL_JUNIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("SENIOR_MISSILE_LEVEL", &ptUserNameLoginSuccU->LEVEL_SENIOR_MISSILE);
	mySqlResReader->GetInt("MIDDLE_MISSILE_LEVEL", &ptUserNameLoginSuccU->LEVEL_MIDDLE_MISSILE);
	mySqlResReader->GetInt("JUNIOR_MISSILE_LEVEL", &ptUserNameLoginSuccU->LEVEL_JUNIOR_MISSILE);
	mySqlResReader->GetInt("SENIOR_RADAR_LEVEL", &ptUserNameLoginSuccU->LEVEL_SENIOR_RADAR);
	mySqlResReader->GetInt("MIDDLE_RADAR_LEVEL", &ptUserNameLoginSuccU->LEVEL_MIDDLE_RADAR);
	mySqlResReader->GetInt("JUNIOR_RADAR_LEVEL", &ptUserNameLoginSuccU->LEVEL_JUNIOR_RADAR);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_LEVEL", &ptUserNameLoginSuccU->LEVEL_SENIOR_CANNONBALL);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_LEVEL", &ptUserNameLoginSuccU->LEVEL_MIDDLE_CANNONBALL);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_LEVEL", &ptUserNameLoginSuccU->LEVEL_JUNIOR_CANNONBALL);

	// 查询用户补给箱信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		CHEST_TABLE_NAME,
		ptUserNameLoginSuccU->USER_ID,
		ptUserNameLoginSuccU->PASSWORD);
	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->GetInt("STAR_NUM", &ptUserNameLoginSuccU->STAR_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_1_NUM", &ptUserNameLoginSuccU->WIN_REWARDS_1_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_2_NUM", &ptUserNameLoginSuccU->WIN_REWARDS_2_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_3_NUM", &ptUserNameLoginSuccU->WIN_REWARDS_3_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_4_NUM", &ptUserNameLoginSuccU->WIN_REWARDS_4_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_5_NUM", &ptUserNameLoginSuccU->WIN_REWARDS_5_NUM);
	mySqlResReader->GetInt("STAR_REWARDS_NUM", &ptUserNameLoginSuccU->STAR_REWARDS_NUM);
	mySqlResReader->GetInt("FREE_REWARDS_NUM", &ptUserNameLoginSuccU->FREE_REWARDS_NUM);
	mySqlResReader->GetInt64("WIN_REWARDS_1_UNLOCK_TIME", &ptUserNameLoginSuccU->WIN_REWARDS_1_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_2_UNLOCK_TIME", &ptUserNameLoginSuccU->WIN_REWARDS_2_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_3_UNLOCK_TIME", &ptUserNameLoginSuccU->WIN_REWARDS_3_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_4_UNLOCK_TIME", &ptUserNameLoginSuccU->WIN_REWARDS_4_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_5_UNLOCK_TIME", &ptUserNameLoginSuccU->WIN_REWARDS_5_UNLOCK_TIME);
	mySqlResReader->GetInt64("LAST_STAR_REWARDS_ACQUIRE_TIME", &ptUserNameLoginSuccU->LAST_STAR_REWARDS_ACQUIRE_TIME);
	mySqlResReader->GetInt64("LAST_FREE_REWARDS_ACQUIRE_TIME", &ptUserNameLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	INT64 currentTime;
	time((time_t*)&currentTime);
	int newFreeChestNum = (currentTime - ptUserNameLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME) / FREE_REWARDS_INTERVAL;
	ptUserNameLoginSuccU->FREE_REWARDS_NUM += newFreeChestNum;
	ptUserNameLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME += newFreeChestNum * FREE_REWARDS_INTERVAL;

	return ptUserNameLoginSuccU;
}

S_PT_BIND_ACCOUNT_LOGIN_SUCC_U* CConnectedUser::db_BindAccountLogin(CMySqlSessionPool mySqlSessionPool, S_PT_BIND_ACCOUNT_LOGIN ptBindAccountLogin)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();
	S_PT_BIND_ACCOUNT_LOGIN_SUCC_U *ptBindAccountLoginSuccU = new S_PT_BIND_ACCOUNT_LOGIN_SUCC_U();
	memset(ptBindAccountLoginSuccU, 0, sizeof(S_PT_BIND_ACCOUNT_LOGIN_SUCC_U));

	// 查询用户账户信息
	sprintf(mySqlSession->mCommand, "select * from %s where %s = '%s' and %s = '%s'", 
		USER_TABLE_NAME, 
		"BIND_ACCOUNT_TYPE", 
		ptBindAccountLogin.BIND_ACCOUNT_TYPE,  
		"BIND_ACCOUNT", 
		ptBindAccountLogin.BIND_ACCOUNT);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("USER_ID", &ptBindAccountLoginSuccU->USER_ID);
	mySqlResReader->GetCHARs("NICKNAME", ptBindAccountLoginSuccU->NICKNAME);
	mySqlResReader->GetCHARs("PASSWORD", ptBindAccountLoginSuccU->PASSWORD);
	mySqlResReader->GetCHARs("BIND_ACCOUNT_TYPE", ptBindAccountLoginSuccU->BIND_ACCOUNT_TYPE);
	mySqlResReader->GetCHARs("BIND_ACCOUNT", ptBindAccountLoginSuccU->BIND_ACCOUNT);
	mySqlResReader->GetCHARs("CELLPHONE_NO", ptBindAccountLoginSuccU->CELLPHONE_NO);
	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptBindAccountLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptBindAccountLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptBindAccountLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptBindAccountLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptBindAccountLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptBindAccountLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptBindAccountLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptBindAccountLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptBindAccountLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptBindAccountLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptBindAccountLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptBindAccountLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptBindAccountLoginSuccU->SESSION_ID);

	// 查询用户装备信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		EQUIPMENT_TABLE_NAME,
		ptBindAccountLoginSuccU->USER_ID,
		ptBindAccountLoginSuccU->PASSWORD);

	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("JUNIOR_PLANE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_JUNIOR_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_PLANE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_MIDDLE_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_PLANE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_SENIOR_PLANE_CARD);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_JUNIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_MIDDLE_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_SENIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_SENIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_MIDDLE_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_JUNIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_MISSILE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_SENIOR_MISSILE_CARD);
	mySqlResReader->GetInt("MIDDLE_MISSILE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_MIDDLE_MISSILE_CARD);
	mySqlResReader->GetInt("JUNIOR_MISSILE_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_JUNIOR_MISSILE_CARD);
	mySqlResReader->GetInt("SENIOR_RADAR_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_SENIOR_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_RADAR_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_MIDDLE_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_RADAR_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_JUNIOR_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_SENIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_MIDDLE_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_CARD_NUM", &ptBindAccountLoginSuccU->NUM_OF_JUNIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_PLANE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_JUNIOR_PLANE);
	mySqlResReader->GetInt("MIDDLE_PLANE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_MIDDLE_PLANE);
	mySqlResReader->GetInt("SENIOR_PLANE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_SENIOR_PLANE);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_JUNIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_MIDDLE_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_SENIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_LEVEL", &ptBindAccountLoginSuccU->LEVEL_SENIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_LEVEL", &ptBindAccountLoginSuccU->LEVEL_MIDDLE_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_LEVEL", &ptBindAccountLoginSuccU->LEVEL_JUNIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("SENIOR_MISSILE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_SENIOR_MISSILE);
	mySqlResReader->GetInt("MIDDLE_MISSILE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_MIDDLE_MISSILE);
	mySqlResReader->GetInt("JUNIOR_MISSILE_LEVEL", &ptBindAccountLoginSuccU->LEVEL_JUNIOR_MISSILE);
	mySqlResReader->GetInt("SENIOR_RADAR_LEVEL", &ptBindAccountLoginSuccU->LEVEL_SENIOR_RADAR);
	mySqlResReader->GetInt("MIDDLE_RADAR_LEVEL", &ptBindAccountLoginSuccU->LEVEL_MIDDLE_RADAR);
	mySqlResReader->GetInt("JUNIOR_RADAR_LEVEL", &ptBindAccountLoginSuccU->LEVEL_JUNIOR_RADAR);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_LEVEL", &ptBindAccountLoginSuccU->LEVEL_SENIOR_CANNONBALL);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_LEVEL", &ptBindAccountLoginSuccU->LEVEL_MIDDLE_CANNONBALL);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_LEVEL", &ptBindAccountLoginSuccU->LEVEL_JUNIOR_CANNONBALL);

	// 查询用户补给箱信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		CHEST_TABLE_NAME,
		ptBindAccountLoginSuccU->USER_ID,
		ptBindAccountLoginSuccU->PASSWORD);
	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("STAR_NUM", &ptBindAccountLoginSuccU->STAR_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_1_NUM", &ptBindAccountLoginSuccU->WIN_REWARDS_1_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_2_NUM", &ptBindAccountLoginSuccU->WIN_REWARDS_2_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_3_NUM", &ptBindAccountLoginSuccU->WIN_REWARDS_3_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_4_NUM", &ptBindAccountLoginSuccU->WIN_REWARDS_4_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_5_NUM", &ptBindAccountLoginSuccU->WIN_REWARDS_5_NUM);
	mySqlResReader->GetInt("STAR_REWARDS_NUM", &ptBindAccountLoginSuccU->STAR_REWARDS_NUM);
	mySqlResReader->GetInt("FREE_REWARDS_NUM", &ptBindAccountLoginSuccU->FREE_REWARDS_NUM);
	mySqlResReader->GetInt64("WIN_REWARDS_1_UNLOCK_TIME", &ptBindAccountLoginSuccU->WIN_REWARDS_1_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_2_UNLOCK_TIME", &ptBindAccountLoginSuccU->WIN_REWARDS_2_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_3_UNLOCK_TIME", &ptBindAccountLoginSuccU->WIN_REWARDS_3_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_4_UNLOCK_TIME", &ptBindAccountLoginSuccU->WIN_REWARDS_4_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_5_UNLOCK_TIME", &ptBindAccountLoginSuccU->WIN_REWARDS_5_UNLOCK_TIME);
	mySqlResReader->GetInt64("LAST_STAR_REWARDS_ACQUIRE_TIME", &ptBindAccountLoginSuccU->LAST_STAR_REWARDS_ACQUIRE_TIME);
	mySqlResReader->GetInt64("LAST_FREE_REWARDS_ACQUIRE_TIME", &ptBindAccountLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	INT64 currentTime;
	time((time_t*)&currentTime);
	int newFreeChestNum = (currentTime - ptBindAccountLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME) / FREE_REWARDS_INTERVAL;
	ptBindAccountLoginSuccU->FREE_REWARDS_NUM += newFreeChestNum;
	ptBindAccountLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME += newFreeChestNum * FREE_REWARDS_INTERVAL;

	return ptBindAccountLoginSuccU;
}

S_PT_CELLPHONE_NO_LOGIN_SUCC_U* CConnectedUser::db_CellphoneNoLogin(CMySqlSessionPool mySqlSessionPool, S_PT_CELLPHONE_NO_LOGIN ptCellphoneNoLogin)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();
	S_PT_CELLPHONE_NO_LOGIN_SUCC_U *ptCellphoneNoLoginSuccU = new S_PT_CELLPHONE_NO_LOGIN_SUCC_U();
	memset(ptCellphoneNoLoginSuccU, 0, sizeof(S_PT_CELLPHONE_NO_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select * from %s where %s = '%s'", 
		USER_TABLE_NAME, 
		"CELLPHONE_NO", 
		ptCellphoneNoLogin.CELLPHONE_NO);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("USER_ID", &ptCellphoneNoLoginSuccU->USER_ID);
	mySqlResReader->GetCHARs("NICKNAME", ptCellphoneNoLoginSuccU->NICKNAME);
	mySqlResReader->GetCHARs("PASSWORD", ptCellphoneNoLoginSuccU->PASSWORD);
	mySqlResReader->GetCHARs("BIND_ACCOUNT_TYPE", ptCellphoneNoLoginSuccU->BIND_ACCOUNT_TYPE);
	mySqlResReader->GetCHARs("BIND_ACCOUNT", ptCellphoneNoLoginSuccU->BIND_ACCOUNT);
	mySqlResReader->GetCHARs("CELLPHONE_NO", ptCellphoneNoLoginSuccU->CELLPHONE_NO);
	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptCellphoneNoLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptCellphoneNoLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptCellphoneNoLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptCellphoneNoLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptCellphoneNoLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptCellphoneNoLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptCellphoneNoLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptCellphoneNoLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptCellphoneNoLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptCellphoneNoLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptCellphoneNoLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptCellphoneNoLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptCellphoneNoLoginSuccU->SESSION_ID);

	// 查询用户装备信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		EQUIPMENT_TABLE_NAME,
		ptCellphoneNoLoginSuccU->USER_ID,
		ptCellphoneNoLoginSuccU->PASSWORD);

	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("JUNIOR_PLANE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_JUNIOR_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_PLANE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_MIDDLE_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_PLANE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_SENIOR_PLANE_CARD);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_JUNIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_MIDDLE_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_SENIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_SENIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_MIDDLE_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_JUNIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_MISSILE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_SENIOR_MISSILE_CARD);
	mySqlResReader->GetInt("MIDDLE_MISSILE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_MIDDLE_MISSILE_CARD);
	mySqlResReader->GetInt("JUNIOR_MISSILE_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_JUNIOR_MISSILE_CARD);
	mySqlResReader->GetInt("SENIOR_RADAR_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_SENIOR_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_RADAR_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_MIDDLE_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_RADAR_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_JUNIOR_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_SENIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_MIDDLE_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_CARD_NUM", &ptCellphoneNoLoginSuccU->NUM_OF_JUNIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_PLANE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_JUNIOR_PLANE);
	mySqlResReader->GetInt("MIDDLE_PLANE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_MIDDLE_PLANE);
	mySqlResReader->GetInt("SENIOR_PLANE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_SENIOR_PLANE);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_JUNIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_MIDDLE_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_SENIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_SENIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_MIDDLE_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_JUNIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("SENIOR_MISSILE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_SENIOR_MISSILE);
	mySqlResReader->GetInt("MIDDLE_MISSILE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_MIDDLE_MISSILE);
	mySqlResReader->GetInt("JUNIOR_MISSILE_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_JUNIOR_MISSILE);
	mySqlResReader->GetInt("SENIOR_RADAR_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_SENIOR_RADAR);
	mySqlResReader->GetInt("MIDDLE_RADAR_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_MIDDLE_RADAR);
	mySqlResReader->GetInt("JUNIOR_RADAR_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_JUNIOR_RADAR);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_SENIOR_CANNONBALL);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_MIDDLE_CANNONBALL);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_LEVEL", &ptCellphoneNoLoginSuccU->LEVEL_JUNIOR_CANNONBALL);

	// 查询用户补给箱信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		CHEST_TABLE_NAME,
		ptCellphoneNoLoginSuccU->USER_ID,
		ptCellphoneNoLoginSuccU->PASSWORD);
	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("STAR_NUM", &ptCellphoneNoLoginSuccU->STAR_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_1_NUM", &ptCellphoneNoLoginSuccU->WIN_REWARDS_1_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_2_NUM", &ptCellphoneNoLoginSuccU->WIN_REWARDS_2_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_3_NUM", &ptCellphoneNoLoginSuccU->WIN_REWARDS_3_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_4_NUM", &ptCellphoneNoLoginSuccU->WIN_REWARDS_4_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_5_NUM", &ptCellphoneNoLoginSuccU->WIN_REWARDS_5_NUM);
	mySqlResReader->GetInt("STAR_REWARDS_NUM", &ptCellphoneNoLoginSuccU->STAR_REWARDS_NUM);
	mySqlResReader->GetInt("FREE_REWARDS_NUM", &ptCellphoneNoLoginSuccU->FREE_REWARDS_NUM);
	mySqlResReader->GetInt64("WIN_REWARDS_1_UNLOCK_TIME", &ptCellphoneNoLoginSuccU->WIN_REWARDS_1_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_2_UNLOCK_TIME", &ptCellphoneNoLoginSuccU->WIN_REWARDS_2_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_3_UNLOCK_TIME", &ptCellphoneNoLoginSuccU->WIN_REWARDS_3_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_4_UNLOCK_TIME", &ptCellphoneNoLoginSuccU->WIN_REWARDS_4_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_5_UNLOCK_TIME", &ptCellphoneNoLoginSuccU->WIN_REWARDS_5_UNLOCK_TIME);
	mySqlResReader->GetInt64("LAST_STAR_REWARDS_ACQUIRE_TIME", &ptCellphoneNoLoginSuccU->LAST_STAR_REWARDS_ACQUIRE_TIME);
	mySqlResReader->GetInt64("LAST_FREE_REWARDS_ACQUIRE_TIME", &ptCellphoneNoLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	INT64 currentTime;
	time((time_t*)&currentTime);
	int newFreeChestNum = (currentTime - ptCellphoneNoLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME) / FREE_REWARDS_INTERVAL;
	ptCellphoneNoLoginSuccU->FREE_REWARDS_NUM += newFreeChestNum;
	ptCellphoneNoLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME += newFreeChestNum * FREE_REWARDS_INTERVAL;

	return ptCellphoneNoLoginSuccU;
}

S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U* CConnectedUser::db_VisitorDefaultLogin(CMySqlSessionPool mySqlSessionPool, S_PT_VISITOR_DEFAULT_LOGIN ptVisitorDefaultLogin)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();
	S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U *ptVisitorDefaultLoginSuccU = new S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U();
	memset(ptVisitorDefaultLoginSuccU, 0, sizeof(S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U));
	sprintf(mySqlSession->mCommand, "select * from %s where %s = %d and %s = '%s'",  
		USER_TABLE_NAME, 
		"USER_ID", 
		ptVisitorDefaultLogin.USER_ID,
		"PASSWORD",
		ptVisitorDefaultLogin.PASSWORD);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();
	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0) {
		delete mySqlResReader;
		return NULL;
	}

	mySqlResReader->ReadRow();
	
	mySqlResReader->GetInt("USER_ID", &ptVisitorDefaultLoginSuccU->USER_ID);
	mySqlResReader->GetCHARs("NICKNAME", ptVisitorDefaultLoginSuccU->NICKNAME);
	mySqlResReader->GetCHARs("PASSWORD", ptVisitorDefaultLoginSuccU->PASSWORD);
	mySqlResReader->GetCHARs("BIND_ACCOUNT_TYPE", ptVisitorDefaultLoginSuccU->BIND_ACCOUNT_TYPE);
	mySqlResReader->GetCHARs("BIND_ACCOUNT", ptVisitorDefaultLoginSuccU->BIND_ACCOUNT);
	mySqlResReader->GetCHARs("CELLPHONE_NO", ptVisitorDefaultLoginSuccU->CELLPHONE_NO);
	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptVisitorDefaultLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptVisitorDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptVisitorDefaultLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptVisitorDefaultLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptVisitorDefaultLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptVisitorDefaultLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptVisitorDefaultLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptVisitorDefaultLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptVisitorDefaultLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptVisitorDefaultLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptVisitorDefaultLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptVisitorDefaultLoginSuccU->SESSION_ID);

	// 查询用户装备信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		EQUIPMENT_TABLE_NAME,
		ptVisitorDefaultLogin.USER_ID,
		ptVisitorDefaultLogin.PASSWORD);

	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0) {
		delete mySqlResReader;
		return NULL;
	}

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("JUNIOR_PLANE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_JUNIOR_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_PLANE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_MIDDLE_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_PLANE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_SENIOR_PLANE_CARD);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_JUNIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_MIDDLE_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_SENIOR_STEALTH_PLANE_CARD);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_SENIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_MIDDLE_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_JUNIOR_ANTI_STEALTH_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_MISSILE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_SENIOR_MISSILE_CARD);
	mySqlResReader->GetInt("MIDDLE_MISSILE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_MIDDLE_MISSILE_CARD);
	mySqlResReader->GetInt("JUNIOR_MISSILE_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_JUNIOR_MISSILE_CARD);
	mySqlResReader->GetInt("SENIOR_RADAR_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_SENIOR_RADAR_CARD);
	mySqlResReader->GetInt("MIDDLE_RADAR_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_MIDDLE_RADAR_CARD);
	mySqlResReader->GetInt("JUNIOR_RADAR_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_JUNIOR_RADAR_CARD);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_SENIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_MIDDLE_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_CARD_NUM", &ptVisitorDefaultLoginSuccU->NUM_OF_JUNIOR_CANNONBALL_CARD);
	mySqlResReader->GetInt("JUNIOR_PLANE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_JUNIOR_PLANE);
	mySqlResReader->GetInt("MIDDLE_PLANE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_MIDDLE_PLANE);
	mySqlResReader->GetInt("SENIOR_PLANE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_SENIOR_PLANE);
	mySqlResReader->GetInt("JUNIOR_STEALTH_PLANE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_JUNIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("MIDDLE_STEALTH_PLANE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_MIDDLE_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_STEALTH_PLANE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_SENIOR_STEALTH_PLANE);
	mySqlResReader->GetInt("SENIOR_ANTI_STEALTH_RADAR_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_SENIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("MIDDLE_ANTI_STEALTH_RADAR_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_MIDDLE_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("JUNIOR_ANTI_STEALTH_RADAR_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_JUNIOR_ANTI_STEALTH_RADAR);
	mySqlResReader->GetInt("SENIOR_MISSILE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_SENIOR_MISSILE);
	mySqlResReader->GetInt("MIDDLE_MISSILE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_MIDDLE_MISSILE);
	mySqlResReader->GetInt("JUNIOR_MISSILE_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_JUNIOR_MISSILE);
	mySqlResReader->GetInt("SENIOR_RADAR_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_SENIOR_RADAR);
	mySqlResReader->GetInt("MIDDLE_RADAR_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_MIDDLE_RADAR);
	mySqlResReader->GetInt("JUNIOR_RADAR_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_JUNIOR_RADAR);
	mySqlResReader->GetInt("SENIOR_CANNONBALL_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_SENIOR_CANNONBALL);
	mySqlResReader->GetInt("MIDDLE_CANNONBALL_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_MIDDLE_CANNONBALL);
	mySqlResReader->GetInt("JUNIOR_CANNONBALL_LEVEL", &ptVisitorDefaultLoginSuccU->LEVEL_JUNIOR_CANNONBALL);

	// 查询用户补给箱信息
	sprintf(mySqlSession->mCommand, "select * from %s where USER_ID = %d and PASSWORD = '%s'", 
		CHEST_TABLE_NAME,
		ptVisitorDefaultLoginSuccU->USER_ID,
		ptVisitorDefaultLoginSuccU->PASSWORD);
	
	MySqlResult = mySqlSession->QUERY_DATA();
	mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );
	
	if(mySqlResReader->GetNumRows() == 0) {
		delete mySqlResReader;
		return NULL;
	}

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("STAR_NUM", &ptVisitorDefaultLoginSuccU->STAR_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_1_NUM", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_1_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_2_NUM", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_2_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_3_NUM", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_3_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_4_NUM", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_4_NUM);
	mySqlResReader->GetInt("WIN_REWARDS_5_NUM", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_5_NUM);
	mySqlResReader->GetInt("STAR_REWARDS_NUM", &ptVisitorDefaultLoginSuccU->STAR_REWARDS_NUM);
	mySqlResReader->GetInt("FREE_REWARDS_NUM", &ptVisitorDefaultLoginSuccU->FREE_REWARDS_NUM);
	mySqlResReader->GetInt64("WIN_REWARDS_1_UNLOCK_TIME", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_1_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_2_UNLOCK_TIME", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_2_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_3_UNLOCK_TIME", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_3_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_4_UNLOCK_TIME", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_4_UNLOCK_TIME);
	mySqlResReader->GetInt64("WIN_REWARDS_5_UNLOCK_TIME", &ptVisitorDefaultLoginSuccU->WIN_REWARDS_5_UNLOCK_TIME);
	mySqlResReader->GetInt64("LAST_STAR_REWARDS_ACQUIRE_TIME", &ptVisitorDefaultLoginSuccU->LAST_STAR_REWARDS_ACQUIRE_TIME);
	mySqlResReader->GetInt64("LAST_FREE_REWARDS_ACQUIRE_TIME", &ptVisitorDefaultLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME);
	
	delete mySqlResReader;

	mySqlSession->FreeSession();

	INT64 currentTime;
	time((time_t*)&currentTime);
	int newFreeChestNum = (currentTime - ptVisitorDefaultLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME) / FREE_REWARDS_INTERVAL;
	ptVisitorDefaultLoginSuccU->FREE_REWARDS_NUM += newFreeChestNum;
	ptVisitorDefaultLoginSuccU->LAST_FREE_REWARDS_ACQUIRE_TIME += newFreeChestNum * FREE_REWARDS_INTERVAL;

	return ptVisitorDefaultLoginSuccU;
}