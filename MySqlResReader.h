#pragma once
#include "mysql.h"

class CMySqlResReader
{
private:
	MYSQL_RES			*mMYSQL_RES;
	MYSQL_FIELD		*mMYSQL_FIELD;
	MYSQL_ROW		mMYSQL_ROW;
public:
	CMySqlResReader(void);
	~CMySqlResReader(void);
	static CMySqlResReader* CreateMySqlResReader(MYSQL_RES *mysql_res);
public:
	BOOL						ReadRow();
	INT							GetNumRows();
	INT							GetNumFields();
	BOOL 						GetInt(const char *key, INT *buffer);
	BOOL						GetFloat(const char *key, FLOAT *buffer);
	BOOL						GetDWORD_PTR(const char *key, DWORD_PTR *buffer);
	BOOL						GetCHARs(const char *key, CHAR* buffer);
	BOOL						GetInt64(const char *key, INT64 *buffer);
};

