#include "stdafx.h"
#include "ConnectedUser.h"
#include "GameCtrlParams.h"

INT CConnectedUser::db_VisitorRegist(CMySqlSessionPool mySqlSessionPool)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();

	// ָ���˻��� USER_NAME Ϊ user_name, NICKNAME Ϊ���ݿ����趨��Ĭ��ֵΪ Visitor
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

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) values (%d, '%s')", EQUIPMENT_TABLE_NAME, USER_ID, "123");
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userEquipmentInfo data fail\n");
	}

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) value (%d, '%s')", CHEST_TABLE_NAME, USER_ID, "123");
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userChestInfo data fail\n");
	}

	mySqlSession->FreeSession();

	return USER_ID;
}

INT CConnectedUser::db_UserNameRegist(CMySqlSessionPool mySqlSessionPool, S_PT_USER_NAME_REGIST ptUserNameRegist)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();

	// ָ���˻��� USER_NAME �� NICKNAME ��Ϊ�ͻ��˴����� NICKNAME
	sprintf(mySqlSession->mCommand, "insert into %s (%s, %s, %s) values ('%s', '%s', '%s')", USER_TABLE_NAME, "USER_NAME", "NICKNAME", "PASSWORD", ptUserNameRegist.NICKNAME, ptUserNameRegist.NICKNAME, ptUserNameRegist.PASSWORD);

	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userAccountInfo data fail\n");
	}

	sprintf(mySqlSession->mCommand, "select LAST_INSERT_ID()");
	MYSQL_RES* QueryResult = mySqlSession->QUERY_DATA();
	MYSQL_ROW row = mysql_fetch_row(QueryResult);
	INT USER_ID = atoi(row[0]);
	mysql_free_result(QueryResult);

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) values (%d, '%s')", EQUIPMENT_TABLE_NAME, USER_ID, ptUserNameRegist.PASSWORD);
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userEquipmentInfo data fail\n");
	}

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) value (%d, '%s')", CHEST_TABLE_NAME, USER_ID, ptUserNameRegist.PASSWORD);
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userChestInfo data fail\n");
	}

	mySqlSession->FreeSession();

	return USER_ID;
}

INT CConnectedUser::db_BindAccountRegist(CMySqlSessionPool mySqlSessionPool, S_PT_BIND_ACCOUNT_REGIST ptBindAccountRegist)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();

	// ָ���˻��� NICKNAME Ϊ stdafx.h �������� DEFAULT_BIND_ACCOUNT_NICKNAME, USER_NAME û�н�������
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

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) values (%d, '%s')", EQUIPMENT_TABLE_NAME, USER_ID, "123");
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userEquipmentInfo data fail\n");
	}

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) value (%d, '%s')", CHEST_TABLE_NAME, USER_ID, "123");
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userChestInfo data fail\n");
	}

	mySqlSession->FreeSession();

	return USER_ID;
}

INT CConnectedUser::db_CellphoneNoRegist(CMySqlSessionPool mySqlSessionPool, S_PT_CELLPHONE_NO_REGIST ptCellphoneNoRegist)
{
	CThreadSync Sync;

	CMySqlSession * mySqlSession = mySqlSessionPool.GetFreeSession();

	// // ָ���˻��� NICKNAME Ϊ stdafx.h �������� DEFAULT_CELLPHONE_NO_NICKNAME, USER_NAME û�н�������
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

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) values (%d, '%s')", EQUIPMENT_TABLE_NAME, USER_ID, "123");
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userEquipmentInfo data fail\n");
	}

	sprintf(mySqlSession->mCommand, "insert into %s (USER_ID, PASSWORD) value (%d, '%s')", CHEST_TABLE_NAME, USER_ID, "123");
	if(!mySqlSession->OPERATE_DATA())
	{
		printf("insert userChestInfo data fail\n");
	}

	mySqlSession->FreeSession();

	return USER_ID;
}