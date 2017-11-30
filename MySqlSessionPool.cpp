#include "stdafx.h"
#include "MySqlSessionPool.h"


CMySqlSessionPool::CMySqlSessionPool(void)
{
}


CMySqlSessionPool::~CMySqlSessionPool(void)
{
}

BOOL CMySqlSessionPool::Begin()
{
	for(int i = 0; i < DB_MAX_CONN; i++) // mysql 默认最多可以同时连接 152 连接
	{
		CMySqlSession *mySqlSession = new CMySqlSession();
		if(!mySqlSession->Begin())
		{
			End();
			return FALSE;
		}

		mMySqlSessionPool.push_back(mySqlSession);
	}

	return TRUE;
}

BOOL CMySqlSessionPool::End()
{
	for(int i = 0; i < mMySqlSessionPool.size(); i++)
	{
		CMySqlSession *mySqlSession = mMySqlSessionPool.at(i);

		while(mySqlSession->GetIsBusy()) {};
		mySqlSession->End();

		delete mySqlSession;
	}

	mMySqlSessionPool.clear();

	return TRUE;
}

CMySqlSession* CMySqlSessionPool::GetFreeSession()
{
	CMySqlSession *mySqlSession = NULL;
	for(int i = 0; i < mMySqlSessionPool.size(); i++)
	{
		mySqlSession = mMySqlSessionPool.at(i);

		if(!mySqlSession)
			continue;

		if(mySqlSession->GetIsBusy())
			continue;

		mySqlSession->SetIsBusy(TRUE);
		return mySqlSession;
	}

	return NULL;
}