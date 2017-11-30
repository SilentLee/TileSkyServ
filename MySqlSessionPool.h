#pragma once
#include <Windows.h>
#include <vector>
#include "MySqlSession.h"

using namespace std;

class CMySqlSessionPool
{
private:
	vector<CMySqlSession*> mMySqlSessionPool;
public:
	CMySqlSessionPool(void);
	~CMySqlSessionPool(void);

	BOOL Begin();
	BOOL End();
	CMySqlSession* GetFreeSession();
public:
	inline int getConnCount() {  return mMySqlSessionPool.size(); }
};

