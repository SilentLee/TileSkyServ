#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"

#include "../Packet/ErrorCode.h"
#include "../Packet/UserProtocol.h"
#include "../Packet/User_PT_Structure.h"
#include "../Packet/User_PT_ReadPacket.h"
#include "../Packet/User_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

INT CGameIocp::db_VisitorRegist()
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();

	// 指定账户中 USER_NAME 为 user_name, NICKNAME 为数据库中设定的默认值为 Visitor
	sprintf(mySqlSession->mCommand, "insert into %s (%s) values ('%s')", USER_TABLE_NAME, "USER_NAME", "user_name");

	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert data fail\n");
	}

	sprintf(mySqlSession->mCommand, "select LAST_INSERT_ID()");
	MYSQL_RES* QueryResult = mySqlSession->QUERY_DATA();
	MYSQL_ROW row = mysql_fetch_row(QueryResult);
	INT USER_ID = atoi(row[0]);
	mysql_free_result(QueryResult);

	mySqlSession->FreeSession();

	return USER_ID;
}

INT CGameIocp::db_UserNameRegist(S_PT_USER_NAME_REGIST ptUserNameRegist)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();

	// 指定账户中 USER_NAME 与 NICKNAME 均为客户端传来的 NICKNAME
	sprintf(mySqlSession->mCommand, "insert into %s (%s, %s, %s) values ('%s', '%s', '%s')", USER_TABLE_NAME, "USER_NAME", "NICKNAME", "PASSWORD", ptUserNameRegist.NICKNAME, ptUserNameRegist.NICKNAME, ptUserNameRegist.PASSWORD);

	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert data fail\n");
	}

	sprintf(mySqlSession->mCommand, "select LAST_INSERT_ID()");
	MYSQL_RES* QueryResult = mySqlSession->QUERY_DATA();
	MYSQL_ROW row = mysql_fetch_row(QueryResult);
	INT USER_ID = atoi(row[0]);
	mysql_free_result(QueryResult);

	mySqlSession->FreeSession();

	return USER_ID;
}

INT CGameIocp::db_BindAccountRegist(S_PT_BIND_ACCOUNT_REGIST ptBindAccountRegist)
{

	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();

	// 指定账户中 NICKNAME 为 stdafx.h 中声明的 DEFAULT_BIND_ACCOUNT_NICKNAME, USER_NAME 没有进行设置
	sprintf(mySqlSession->mCommand, "insert into %s (%s, %s, %s) values ('%s', '%s', '%s')", USER_TABLE_NAME, "BIND_ACCOUNT_TYPE", "BIND_ACCOUNT", "NICKNAME", ptBindAccountRegist.BIND_ACCOUNT_TYPE, ptBindAccountRegist.BIND_ACCOUNT, DEFAULT_BIND_ACCOUNT_NICKNAME);

	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert data fail\n");
	}

	sprintf(mySqlSession->mCommand, "select LAST_INSERT_ID()");
	MYSQL_RES* QueryResult = mySqlSession->QUERY_DATA();
	MYSQL_ROW row = mysql_fetch_row(QueryResult);
	INT USER_ID = atoi(row[0]);
	mysql_free_result(QueryResult);

	mySqlSession->FreeSession();

	return USER_ID;
}

INT CGameIocp::db_CellphoneNoRegist(S_PT_CELLPHONE_NO_REGIST ptCellphoneNoRegist)
{

	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();

	// // 指定账户中 NICKNAME 为 stdafx.h 中声明的 DEFAULT_CELLPHONE_NO_NICKNAME, USER_NAME 没有进行设置
	sprintf(mySqlSession->mCommand, "insert into %s (%s, %s) values ('%s', '%s')", USER_TABLE_NAME, "CELLPHONE_NO", "NICKNAME", ptCellphoneNoRegist.CELLPHONE_NO, DEFAULT_CELLPHONE_NO_NICKNAME);
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert data fail\n");
	}

	sprintf(mySqlSession->mCommand, "select LAST_INSERT_ID()");
	MYSQL_RES* QueryResult = mySqlSession->QUERY_DATA();
	MYSQL_ROW row = mysql_fetch_row(QueryResult);
	INT USER_ID = atoi(row[0]);
	mysql_free_result(QueryResult);

	mySqlSession->FreeSession();

	return USER_ID;
}

S_PT_USER_NAME_LOGIN_SUCC_U* CGameIocp::db_UserNameLogin(S_PT_USER_NAME_LOGIN ptUserNameLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_USER_NAME_LOGIN_SUCC_U *ptUserNameLoginSuccU = new S_PT_USER_NAME_LOGIN_SUCC_U();
	memset(ptUserNameLoginSuccU, 0, sizeof(S_PT_USER_NAME_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = '%s' and %s = '%s'", 
		"USER_ID", 
		"NICKNAME",
		"PASSWORD",
		"BIND_ACCOUNT_TYPE",
		"BIND_ACCOUNT",
		"CELLPHONE_NO",
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
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

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptUserNameLoginSuccU;
}

S_PT_BIND_ACCOUNT_LOGIN_SUCC_U* CGameIocp::db_BindAccountLogin(S_PT_BIND_ACCOUNT_LOGIN ptBindAccountLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_BIND_ACCOUNT_LOGIN_SUCC_U *ptBindAccountLoginSuccU = new S_PT_BIND_ACCOUNT_LOGIN_SUCC_U();
	memset(ptBindAccountLoginSuccU, 0, sizeof(S_PT_BIND_ACCOUNT_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = '%s' and %s = '%s'", 
		"USER_ID", 
		"NICKNAME",
		"PASSWORD",
		"BIND_ACCOUNT_TYPE",
		"BIND_ACCOUNT",
		"CELLPHONE_NO",
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
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

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptBindAccountLoginSuccU;
}

S_PT_CELLPHONE_NO_LOGIN_SUCC_U* CGameIocp::db_CellphoneNoLogin(S_PT_CELLPHONE_NO_LOGIN ptCellphoneNoLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_CELLPHONE_NO_LOGIN_SUCC_U *ptCellphoneNoLoginSuccU = new S_PT_CELLPHONE_NO_LOGIN_SUCC_U();
	memset(ptCellphoneNoLoginSuccU, 0, sizeof(S_PT_CELLPHONE_NO_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = '%s'", 
		"USER_ID", 
		"NICKNAME",
		"PASSWORD",
		"BIND_ACCOUNT_TYPE",
		"BIND_ACCOUNT",
		"CELLPHONE_NO",
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
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

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptCellphoneNoLoginSuccU;
}

S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U* CGameIocp::db_VisitorDefaultLogin(S_PT_VISITOR_DEFAULT_LOGIN ptVisitorDefaultLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U *ptVisitorDefaultLoginSuccU = new S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U();
	memset(ptVisitorDefaultLoginSuccU, 0, sizeof(S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = %d and %s = '%s'", 
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
		USER_TABLE_NAME, 
		"USER_ID", 
		ptVisitorDefaultLogin.USER_ID,
		"PASSWORD",
		ptVisitorDefaultLogin.PASSWORD);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();
	
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

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptVisitorDefaultLoginSuccU;
}

S_PT_USER_NAME_DEFAULT_LOGIN_SUCC_U* CGameIocp::db_UserNameDefaultLogin(S_PT_USER_NAME_DEFAULT_LOGIN ptUserNameDefaultLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_USER_NAME_DEFAULT_LOGIN_SUCC_U *ptUserNameDefaultLoginSuccU = new S_PT_USER_NAME_DEFAULT_LOGIN_SUCC_U();
	memset(ptUserNameDefaultLoginSuccU, 0, sizeof(S_PT_USER_NAME_DEFAULT_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = %d and %s = '%s' and %s = '%s'", 
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
		USER_TABLE_NAME, 
		"USER_ID", 
		ptUserNameDefaultLogin.USER_ID,
		"USER_NAME",
		ptUserNameDefaultLogin.USER_NAME,
		"PASSWORD",
		ptUserNameDefaultLogin.PASSWORD);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptUserNameDefaultLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptUserNameDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptUserNameDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptUserNameDefaultLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptUserNameDefaultLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptUserNameDefaultLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptUserNameDefaultLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptUserNameDefaultLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptUserNameDefaultLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptUserNameDefaultLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptUserNameDefaultLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptUserNameDefaultLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptUserNameDefaultLoginSuccU->SESSION_ID);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptUserNameDefaultLoginSuccU;
}

S_PT_BIND_ACCOUNT_DEFAULT_LOGIN_SUCC_U* CGameIocp::db_BindAccountDefaultLogin
	(S_PT_BIND_ACCOUNT_DEFAULT_LOGIN ptBindAccountDefaultLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_BIND_ACCOUNT_DEFAULT_LOGIN_SUCC_U *ptBindAccountDefaultLoginSuccU = new S_PT_BIND_ACCOUNT_DEFAULT_LOGIN_SUCC_U();
	memset(ptBindAccountDefaultLoginSuccU, 0, sizeof(S_PT_BIND_ACCOUNT_DEFAULT_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = %d and %s = '%s' and %s = '%s'", 
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
		USER_TABLE_NAME, 
		"USER_ID", 
		ptBindAccountDefaultLogin.USER_ID,
		"BIND_ACCOUNT_TYPE",
		ptBindAccountDefaultLogin.BIND_ACCOUNT_TYPE,
		"BIND_ACCOUNT",
		ptBindAccountDefaultLogin.BIND_ACCOUNT);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptBindAccountDefaultLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptBindAccountDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptBindAccountDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptBindAccountDefaultLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptBindAccountDefaultLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptBindAccountDefaultLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptBindAccountDefaultLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptBindAccountDefaultLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptBindAccountDefaultLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptBindAccountDefaultLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptBindAccountDefaultLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptBindAccountDefaultLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptBindAccountDefaultLoginSuccU->SESSION_ID);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptBindAccountDefaultLoginSuccU;
}

S_PT_CELLPHONE_NO_DEFALUT_LOGIN_SUCC_U* CGameIocp::db_CellphoneNoDefaultLogin
	(S_PT_CELLPHONE_NO_DEFAULT_LOGIN ptCellphoneNoDefaultLogin)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_CELLPHONE_NO_DEFALUT_LOGIN_SUCC_U *ptCellphoneNoDefaultLoginSuccU = new S_PT_CELLPHONE_NO_DEFALUT_LOGIN_SUCC_U();
	memset(ptCellphoneNoDefaultLoginSuccU, 0, sizeof(S_PT_CELLPHONE_NO_DEFALUT_LOGIN_SUCC_U));

	sprintf(mySqlSession->mCommand, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = %d and %s = '%s'", 
		"INTEGRAL_POINTS",
		"LEVEL",
		"DIAMONDS",
		"COINS",
		"WIN_TIMES",
		"BATTLE_TIMES",
		"WIN_RATE",
		"SHOT_DOWN_PLANES",
		"SHOT_DOWN_JUNIOR_PLANES",
		"SHOT_DOWN_MIDDLE_PLANES",
		"SHOT_DOWN_SENIOR_PLANES",
		USER_TABLE_NAME, 
		"USER_ID", 
		ptCellphoneNoDefaultLogin.USER_ID,
		"CELLPHONE_NO",
		ptCellphoneNoDefaultLogin.CELLPHONE_NO);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetInt("INTEGRAL_POINTS", &ptCellphoneNoDefaultLoginSuccU->INTEGRAL_POINTS);
	//mySqlResReader->GetCHARs("LEVEL", ptCellphoneNoDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("LEVEL", &ptCellphoneNoDefaultLoginSuccU->LEVEL);
	mySqlResReader->GetInt("DIAMONDS", &ptCellphoneNoDefaultLoginSuccU->DIAMONDS);
	mySqlResReader->GetInt("COINS", &ptCellphoneNoDefaultLoginSuccU->COINS);
	mySqlResReader->GetInt("WIN_TIMES", &ptCellphoneNoDefaultLoginSuccU->WIN_TIMES);
	mySqlResReader->GetInt("BATTLE_TIMES", &ptCellphoneNoDefaultLoginSuccU->BATTLE_TIMES);
	mySqlResReader->GetFloat("WIN_RATE", &ptCellphoneNoDefaultLoginSuccU->WIN_RATE);
	mySqlResReader->GetInt("SHOT_DOWN_PLANES", &ptCellphoneNoDefaultLoginSuccU->SHOT_DOWN_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_JUNIOR_PLANES", &ptCellphoneNoDefaultLoginSuccU->SHOT_DOWN_JUNIOR_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_MIDDLE_PLANES", &ptCellphoneNoDefaultLoginSuccU->SHOT_DOWN_MIDDLE_PLANES);
	mySqlResReader->GetInt("SHOT_DOWN_SENIOR_PLANES", &ptCellphoneNoDefaultLoginSuccU->SHOT_DOWN_SENIOR_PLANES);
	mySqlResReader->GetDWORD_PTR("SESSION_ID", &ptCellphoneNoDefaultLoginSuccU->SESSION_ID);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptCellphoneNoDefaultLoginSuccU;
}

S_PT_ADD_NICKNAME_SUCC_U* CGameIocp::db_AddNickname(S_PT_ADD_NICKNAME ptAddNickname)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_ADD_NICKNAME_SUCC_U *ptAddNicknameSuccU = new S_PT_ADD_NICKNAME_SUCC_U();
	memset(ptAddNicknameSuccU, 0, sizeof(S_PT_ADD_NICKNAME_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = '%s' where %s = %d", USER_TABLE_NAME, "NICKNAME", ptAddNickname.NICKNAME, "USER_ID", ptAddNickname.USER_ID);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	mySqlSession = mMySqlSessionPool.GetFreeSession();

	sprintf(mySqlSession->mCommand, "select %s from %s where %s = %d", 
		"NICKNAME",
		USER_TABLE_NAME, 
		"USER_ID",
		ptAddNickname.USER_ID);


	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetCHARs("NICKNAME", ptAddNicknameSuccU->NICKNAME);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptAddNicknameSuccU;
}

S_PT_ADD_BIND_ACCOUNT_SUCC_U* CGameIocp::db_AddBindAccount(S_PT_ADD_BIND_ACCOUNT ptAddBindAccount)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_ADD_BIND_ACCOUNT_SUCC_U *ptAddBindAccountSuccU = new S_PT_ADD_BIND_ACCOUNT_SUCC_U();
	memset(ptAddBindAccountSuccU, 0, sizeof(S_PT_ADD_BIND_ACCOUNT_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = '%s', %s = '%s' where %s = %d", USER_TABLE_NAME, "BIND_ACCOUNT_TYPE", ptAddBindAccount.BIND_ACCOUNT_TYPE, "BIND_ACCOUNT", ptAddBindAccount.BIND_ACCOUNT_TYPE, "USER_ID", ptAddBindAccount.USER_ID);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	mySqlSession = mMySqlSessionPool.GetFreeSession();

	sprintf(mySqlSession->mCommand, "select %s, %s from %s where %s = %d", 
		"BIND_ACCOUNT_TYPE",
		"BIND_ACCOUNT",
		USER_TABLE_NAME, 
		"USER_ID",
		ptAddBindAccount.USER_ID);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetCHARs("BIND_ACCOUNT_TYPE", ptAddBindAccountSuccU->BIND_ACCOUNT_TYPE);
	mySqlResReader->GetCHARs("BIND_ACCOUNT", ptAddBindAccountSuccU->BIND_ACCOUNT);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptAddBindAccountSuccU;
}

S_PT_ADD_CELLPHONE_NO_SUCC_U* CGameIocp::db_AddCellphoneNo(S_PT_ADD_CELLPHONE_NO ptAddCellphoneNo)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_ADD_CELLPHONE_NO_SUCC_U *ptAddCellphoneNoSuccU = new S_PT_ADD_CELLPHONE_NO_SUCC_U();
	memset(ptAddCellphoneNoSuccU, 0, sizeof(S_PT_ADD_CELLPHONE_NO_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = '%s' where %s = %d", USER_TABLE_NAME, "CELLPHONE_NO", ptAddCellphoneNo.CELLPHONE_NO, "USER_ID", ptAddCellphoneNo.USER_ID);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	mySqlSession = mMySqlSessionPool.GetFreeSession();

	sprintf(mySqlSession->mCommand, "select %s from %s where %s = %d", 
		"CELLPHONE_NO",
		USER_TABLE_NAME, 
		"USER_ID",
		ptAddCellphoneNo.USER_ID);

	MYSQL_RES* MySqlResult = mySqlSession->QUERY_DATA();

	CMySqlResReader* mySqlResReader = CMySqlResReader::CreateMySqlResReader( MySqlResult );

	if(mySqlResReader->GetNumRows() == 0)
		return NULL;

	mySqlResReader->ReadRow();

	mySqlResReader->GetCHARs("CELLPHONE_NO", ptAddCellphoneNoSuccU->CELLPHONE_NO);

	delete mySqlResReader;

	mySqlSession->FreeSession();

	return ptAddCellphoneNoSuccU;
}

S_PT_UPDATE_PASSWORD_SUCC_U* CGameIocp::db_UpdatePassword(S_PT_UPDATE_PASSWORD ptUpdatePassword)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_UPDATE_PASSWORD_SUCC_U *ptUpdatePasswordSuccU = new S_PT_UPDATE_PASSWORD_SUCC_U();
	memset(ptUpdatePasswordSuccU, 0, sizeof(S_PT_UPDATE_PASSWORD_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = '%s' where %s = %d and %s = '%s'", USER_TABLE_NAME, "PASSWORD", ptUpdatePassword.NEW_PASSWORD, "USER_ID", ptUpdatePassword.USER_ID, "PASSWORD", ptUpdatePassword.OLD_PASSWORD);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	memcpy(ptUpdatePasswordSuccU->NEW_PASSWORD, ptUpdatePassword.NEW_PASSWORD, 32);

	mySqlSession->FreeSession();

	return ptUpdatePasswordSuccU;
}

S_PT_UPDATE_NICKNAME_SUCC_U* CGameIocp::db_UpdateNickname(S_PT_UPDATE_NICKNAME ptUpdateNickname)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_UPDATE_NICKNAME_SUCC_U *ptUpdateNicknameSuccU = new S_PT_UPDATE_NICKNAME_SUCC_U();
	memset(ptUpdateNicknameSuccU, 0, sizeof(S_PT_UPDATE_NICKNAME_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = '%s' where %s = %d and %s = '%s'", USER_TABLE_NAME, "NICKNAME", ptUpdateNickname.NEW_NICKNAME, "USER_ID", ptUpdateNickname.USER_ID, "NICKNAME", ptUpdateNickname.OLD_NICKNAME);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	memcpy(ptUpdateNicknameSuccU->NEW_NICKNAME, ptUpdateNickname.NEW_NICKNAME, 32);

	mySqlSession->FreeSession();

	return ptUpdateNicknameSuccU;
}

S_PT_UPDATE_CELLPHONE_NO_SUCC_U* CGameIocp::db_UpdateCellphoneNo(S_PT_UPDATE_CELLPHONE_NO ptUpdateCellphoneNo)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_UPDATE_CELLPHONE_NO_SUCC_U *ptUpdateCellphoneNoSuccU = new S_PT_UPDATE_CELLPHONE_NO_SUCC_U();
	memset(ptUpdateCellphoneNoSuccU, 0, sizeof(S_PT_UPDATE_CELLPHONE_NO_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = '%s' where %s = %d", USER_TABLE_NAME, "CELLPHONE_NO", ptUpdateCellphoneNo.NEW_CELLPHONE_NO, "USER_ID", ptUpdateCellphoneNo.USER_ID);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	memcpy(ptUpdateCellphoneNoSuccU->NEW_CELLPHONE_NO, ptUpdateCellphoneNo.NEW_CELLPHONE_NO, 32);

	mySqlSession->FreeSession();

	return ptUpdateCellphoneNoSuccU;
}

S_PT_UPDATE_RECORD_SUCC_U* CGameIocp::db_UpdateRecord(S_PT_UPDATE_RECORD ptUpdateRecord)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_UPDATE_RECORD_SUCC_U *ptUpdateRecordSuccU = new S_PT_UPDATE_RECORD_SUCC_U();
	memset(ptUpdateRecordSuccU, 0, sizeof(S_PT_UPDATE_RECORD_SUCC_U));

	sprintf(mySqlSession->mCommand, "update %s set %s = %d, %s = %d, %s = %d, %s = %d, %s = %d, %s = %d, %s = %f,  %s = %d, %s = %d, %s = %d, %s = %d where %s = %d", 
		USER_TABLE_NAME, 
		"INTEGRAL_POINTS",
		ptUpdateRecord.INTEGRAL_POINTS,
		"LEVEL",
		ptUpdateRecord.LEVEL,
		"DIAMONDS",
		ptUpdateRecord.DIAMONDS,
		"COINS",
		ptUpdateRecord.COINS,
		"WIN_TIMES",
		ptUpdateRecord.WIN_TIMES,
		"BATTLE_TIMES",
		ptUpdateRecord.BATTLE_TIMES,
		"WIN_RATE",
		ptUpdateRecord.WIN_RATE,
		"SHOT_DOWN_PLANES",
		ptUpdateRecord.SHOT_DOWN_PLANES,
		"SHOT_DOWN_JUNIOR_PLANES",
		ptUpdateRecord.SHOT_DOWN_JUNIOR_PLANES,
		"SHOT_DOWN_MIDDLE_PLANES",
		ptUpdateRecord.SHOT_DOWN_MIDDLE_PLANES,
		"SHOT_DOWN_SENIOR_PLANES",
		ptUpdateRecord.SHOT_DOWN_SENIOR_PLANES,
		"USER_ID", 
		ptUpdateRecord.USER_ID);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	ptUpdateRecordSuccU->INTEGRAL_POINTS = ptUpdateRecord.INTEGRAL_POINTS;
	//memcpy(ptUpdateRecordSuccU->LEVEL, ptUpdateRecord.LEVEL, 32);
	//sprintf(ptUpdateRecordSuccU->LEVEL, ptUpdateRecord.LEVEL);
	ptUpdateRecordSuccU->LEVEL = ptUpdateRecord.LEVEL;
	ptUpdateRecordSuccU->DIAMONDS = ptUpdateRecord.DIAMONDS;
	ptUpdateRecordSuccU->COINS = ptUpdateRecord.COINS;
	ptUpdateRecordSuccU->WIN_TIMES = ptUpdateRecord.WIN_TIMES;
	ptUpdateRecordSuccU->BATTLE_TIMES = ptUpdateRecord.BATTLE_TIMES;
	ptUpdateRecordSuccU->WIN_RATE = ptUpdateRecord.WIN_RATE;
	ptUpdateRecordSuccU->SHOT_DOWN_PLANES = ptUpdateRecord.SHOT_DOWN_PLANES;
	ptUpdateRecordSuccU->SHOT_DOWN_JUNIOR_PLANES = ptUpdateRecord.SHOT_DOWN_JUNIOR_PLANES;
	ptUpdateRecordSuccU->SHOT_DOWN_MIDDLE_PLANES = ptUpdateRecord.SHOT_DOWN_MIDDLE_PLANES;
	ptUpdateRecordSuccU->SHOT_DOWN_SENIOR_PLANES = ptUpdateRecord.SHOT_DOWN_SENIOR_PLANES;

	mySqlSession->FreeSession();

	return ptUpdateRecordSuccU;
}









































