#include "stdafx.h"
#include "MySqlResReader.h"


CMySqlResReader::CMySqlResReader(void)
{
}


CMySqlResReader::~CMySqlResReader(void)
{
	mysql_free_result(mMYSQL_RES);
}

CMySqlResReader* CMySqlResReader::CreateMySqlResReader(MYSQL_RES *mysql_res)
{
	CMySqlResReader* mysql_res_reader = new CMySqlResReader();
	mysql_res_reader->mMYSQL_RES = mysql_res;
	mysql_res_reader->mMYSQL_FIELD = mysql_fetch_fields(mysql_res);
	
	return mysql_res_reader;
}

INT CMySqlResReader::GetNumRows()
{
	return mysql_num_rows(mMYSQL_RES);
}

INT CMySqlResReader::GetNumFields()
{
	return mysql_num_fields(mMYSQL_RES);
}

BOOL CMySqlResReader::ReadRow()
{
	mMYSQL_ROW = mysql_fetch_row(mMYSQL_RES);

	if(mMYSQL_ROW)
		return TRUE;

	return FALSE;
}

BOOL CMySqlResReader::GetInt(const char *key, INT *buffer)
{
	for(unsigned int i = 0; i < mysql_num_fields(mMYSQL_RES); i++)
	{
		if(!strcmp(key,  mMYSQL_FIELD[i].name))
		{
			//memcpy(buffer, mMYSQL_ROW[i], sizeof(INT));
			*buffer = atoi(mMYSQL_ROW[i]);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMySqlResReader::GetFloat(const char *key, FLOAT *buffer)
{
	for(unsigned int i = 0; i < mysql_num_fields(mMYSQL_RES); i++)
	{
		if(!strcmp(key,  mMYSQL_FIELD[i].name))
		{
			//memcpy(buffer, mMYSQL_ROW[i], sizeof(FLOAT));
			*buffer = atof(mMYSQL_ROW[i]);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMySqlResReader::GetDWORD_PTR(const char *key, DWORD_PTR *buffer)
{
	for(unsigned int i = 0; i < mysql_num_fields(mMYSQL_RES); i++)
	{
		if(!strcmp(key,  mMYSQL_FIELD[i].name))
		{
			//memcpy(buffer, mMYSQL_ROW[i], sizeof(DWORD_PTR));
			memcpy(buffer, mMYSQL_ROW[i], sizeof( mMYSQL_ROW[i]));
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMySqlResReader::GetCHARs(const char *key, CHAR* buffer)
{
	for(unsigned int i = 0; i < mysql_num_fields(mMYSQL_RES); i++)
	{
		if(!strcmp(key,  mMYSQL_FIELD[i].name))
		{
			memcpy(buffer, mMYSQL_ROW[i], 32);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMySqlResReader::GetInt64(const char *key, INT64 *buffer)
{
	for(unsigned int i = 0; i < mysql_num_fields(mMYSQL_RES); i++)
	{
		if(!strcmp(key,  mMYSQL_FIELD[i].name))
		{
			//memcpy(buffer, mMYSQL_ROW[i], sizeof(INT));
			//memcpy(buffer,  mMYSQL_ROW[i], sizeof(INT64));
			*buffer = _atoi64(mMYSQL_ROW[i]);
			return TRUE;
		}
	}

	return FALSE;
}
