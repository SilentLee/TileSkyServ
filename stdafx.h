// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

//#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
#include "Global.h"

#define DEFAULT_PORT					6000
#define MAX_USER						100

#define LOG_OK							_T("[ ok ]\n")
#define LOG_FAIL						_T("[ fail ]\n\n")

#define CONFIG_FILENAME										_T("./config.ini")

#define	DB_USER														"root"
#define	DB_PASSWORD											"Lxl18911553200"
#define	DB_IP																"127.0.0.1"
#define	DB_PORT														3306
#define	DB_NAME														"tile_sky"
#define	USER_TABLE_NAME									"user_table"
#define	EQUIPMENT_TABLE_NAME						"equipment_table"
#define	CHEST_TABLE_NAME								"chest_table"
#define	DB_MAX_CONN											50

#define DEFAULT_USER_NAME									"user_name"
#define DEFAULT_USER_PASSWORD						"123"
#define DEFAULT_BIND_ACCOUNT_TYPE					"bind_account_type"
#define DEFAULT_BIND_ACCOUNT								"bind_account"
#define DEFAULT_BIND_ACCOUNT_NICKNAME		"bind_account_user"
#define DEFAULT_CELLPHONE_NO_NICKNAME		"cellphone_no_user"
#define DEFAULT_CELLPHONE_NO							"13900000000"