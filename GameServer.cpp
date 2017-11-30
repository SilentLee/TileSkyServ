// Server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"

#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "MiniDump.h"

#include "AutodetectMemoryLeak.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

//DWORD	DEFAULT_GAMETIME					= 300;
//DWORD	DEFAULT_MAX_HP						= 50;
//DWORD	DEFAULT_MAX_AP						= 50;
//DWORD	DEFAULT_SPECIAL_DAMAGE				= 20;
//DWORD	DEFAULT_SPECIAL_AP					= 20;
//DWORD	DEFAULT_DAMAGE						= 10;
//DWORD	DEFAULT_AP							= 10;
//DWORD	DEFAULT_CATCH_AP					= 5;
//DWORD	DEFAULT_SPECIAL_CATCH_AP			= 10;
//DWORD	DEFAULT_SPEND_AP					= 10;
//DWORD	DEFAULT_EXP							= 10;
//DWORD	DEFAULT_GAMEMONEY					= 10;
//
//DWORD	DEFAULT_EXPLOSION_TIME_FOR_PERSION	= 5;
//DWORD	DEFAULT_EXPLOSION_TIME_FOR_TEAM		= 10;
//DWORD	DEFAULT_EXPLISION_ALRAM_TIME		= 3;
//DWORD	DEFAULT_TURNOVER_TIME				= 10;
//
//FLOAT	DOGS000_ADVANTAGE					= 1.0F;
//FLOAT	DOGS001_ADVANTAGE					= 1.0F;
//FLOAT	DOGS002_ADVANTAGE					= 1.0F;
//FLOAT	DOGS003_ADVANTAGE					= 1.0F;
//FLOAT	DOGS004_ADVANTAGE					= 1.0F;
//FLOAT	DOGS005_ADVANTAGE					= 1.0F;
//FLOAT	DOGS006_ADVANTAGE					= 1.0F;
//FLOAT	DOGS007_ADVANTAGE					= 1.0F;

int _tmain(int argc, _TCHAR* argv[])
{
	CLog::WriteLog(_T("# TileSky Game Server Startup #\n\n"));

	CLog::WriteLogNoDate(_T("+ Prepare Startup\n"));
	CLog::WriteLogNoDate(_T("	- Winsock2 DLL loading		: "));

	// �����׽��ְ汾
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);
	CLog::WriteLogNoDate(LOG_OK);

	CoInitialize(NULL);

	// ��ʼ�� MiniDump
	CLog::WriteLogNoDate(_T("	- MiniDump System Begin		: "));
	if (CMiniDump::Begin())
		CLog::WriteLogNoDate(LOG_OK);
	else
	{
		CLog::WriteLog(LOG_FAIL); // ��ʼ�� MiniDump ʧ��

		CLog::WriteLog(_T("# TileSky Game Server Stoped... #\n"));
		CLog::WriteLogNoDate(_T("Press any key... \n"));
		getchar(); // ���������������

		return 0;
	}

	// ��ʼ�� TCP IOCP
	CGameIocp GameIocp;

	CLog::WriteLogNoDate(_T("+ GameIocp Startup\n"));
	CLog::WriteLogNoDate(_T("	- GameIocp Begin		: "));
	if (GameIocp.Begin())
		CLog::WriteLogNoDate(LOG_OK); // TCP IOCP ��ʼ���ɹ�
	else
	{
		WSACleanup(); // TCP IOCP ��ʼ��ʧ��, ����׽��ְ汾��Ϣ

		CMiniDump::End(); // TCP IOCP ��ʼ��ʧ��, ���� MiniDump

		CLog::WriteLogNoDate(LOG_FAIL);

		CLog::WriteLog(_T("# TileSky Game Server Stoped... #\n"));
		CLog::WriteLogNoDate(_T("Press any key... \n")); 
		getchar(); // ���������������

		return 0;
	}

	CLog::WriteLogNoDate(_T("\n")); // ��������ɳ�ʼ��, ���гɹ�
	CLog::WriteLog(_T("# TileSky Game Server Running... #\n"));
	getchar(); // �������ֹͣ������

	CLog::WriteLogNoDate(_T("+ Prepare Stop\n"));
	GameIocp.End(); // ���� TCP IOCP
	CLog::WriteLogNoDate(_T("	- GameIocp End			: [ ok ]\n"));

	WSACleanup(); // ����׽��ְ汾��Ϣ
	CLog::WriteLogNoDate(_T("	- Winsock2 DLL unloading	: [ ok ]\n"));

	CMiniDump::End(); // ���� MiniDump
	CLog::WriteLogNoDate(_T("	- MiniDump System End		: [ ok ]\n\n"));

	CoUninitialize();

	CLog::WriteLog(_T("# TileServer Game Server Stoped... #\n"));
	CLog::WriteLogNoDate(_T("Press any key... \n"));
	getchar();

	return 0;
}
