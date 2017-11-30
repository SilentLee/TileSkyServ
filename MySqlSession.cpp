#include "stdafx.h"
#include "MySqlSession.h"


CMySqlSession::CMySqlSession(void)
{
	mDataBaseConnection = NULL;
	IsBusy = FALSE;
}


CMySqlSession::~CMySqlSession(void)
{
}

BOOL CMySqlSession::Begin()
{
	CThreadSync Sync;

	IsBusy = TRUE;
	
	mDataBaseConnection = mysql_init((MYSQL*) 0);

	if(mDataBaseConnection == NULL)
	{
		_tprintf(_T("##MySqlSession#Begin: mysql_init() fail\n"));
		
		return FALSE;
	}
	
	if (!mysql_real_connect(mDataBaseConnection, DB_IP, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0)) 
	 {
		 _tprintf(_T("##MySqlSession#Begin: mysql_real_connect() fail\n"));

		 return FALSE;
	 } 

	if(mysql_select_db(mDataBaseConnection,DB_NAME))
	{
		_tprintf(_T("##MySqlSession#Begin: mysql_select_db() fail\n"));

		return FALSE;
	}

	IsBusy = FALSE;

	 return TRUE;
}

BOOL CMySqlSession::End()
{
	CThreadSync Sync;
	mysql_close(mDataBaseConnection);

	return TRUE;
}

BOOL CMySqlSession::GetIsBusy() 
{
	//CThreadSync Sync; 

	/*if(!IsBusy) 
	{
		IsBusy = TRUE; 
		return FALSE; 
	}

	return TRUE;*/
	return IsBusy;
}

BOOL CMySqlSession::OPERATE_DATA()
{
	CThreadSync Sync;

	if(!IsBusy) {
		printf("##MySqlSession#IsBusy = FALSE\n");
		return FALSE;
	}

	int result = mysql_real_query(mDataBaseConnection, mCommand, strlen(mCommand));

	if (result)
	{
		printf("##MySqlSession#INSERT_DATA: Error making query: %s !!!\n", mysql_error(mDataBaseConnection));
		return FALSE;
	}
		
	printf("##MySqlSession#INSERT_DATA: %s executed!!!\n", mCommand);

	memset(mCommand, 0, MAX_BUFFER_LENGTH);

	//IsBusy = FALSE;

	return TRUE;
}

MYSQL_RES* CMySqlSession::QUERY_DATA()
{
	CThreadSync Sync;

	if(!IsBusy) {
		printf("##MySqlSession#IsBusy = FALSE\n");
		return NULL;
	}

	int result = mysql_real_query(mDataBaseConnection, mCommand, strlen(mCommand));

	if (result)
	{
		printf("##MySqlSession#QUERY_DATA: Error making query: %s !!!\n", mysql_error(mDataBaseConnection));
		return NULL;
	}
	
	printf("##MySqlSession#QUERY_DATA: %s executed!!!\n", mCommand);

	memset(mCommand, 0, MAX_BUFFER_LENGTH);

	//IsBusy = FALSE;

	MYSQL_RES *data = mysql_store_result(mDataBaseConnection);

	return data;
}