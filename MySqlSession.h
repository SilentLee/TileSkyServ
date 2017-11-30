#pragma once
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "mysql.h"

class CMySqlSession	:	public CMultiThreadSync<CMySqlSession>
{
private:
	MYSQL						*mDataBaseConnection;
	BOOL						IsBusy;
public:
	char mCommand[MAX_BUFFER_LENGTH];
public:
	CMySqlSession(void);
	~CMySqlSession(void);
	BOOL						Begin();
	BOOL						End();
	BOOL						OPERATE_DATA();
	MYSQL_RES*			QUERY_DATA();

public:
	inline VOID				FreeSession() {CThreadSync Sync; IsBusy = FALSE; }

	void							SetIsBusy(BOOL isBusy){IsBusy = isBusy;};
	BOOL						GetIsBusy();
};
