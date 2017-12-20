#include "stdafx.h"

#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "Stream.h"
#include "IniFile.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

// �� GameIocp ����,�൱�ڿ����� 3 ���߳�
// 1. ���� CIocp �е� WorkerThread, ���ڹ��� IOCP ������ͨ��
// 2. KeepThreadCallback, ���ڼ��ͻ����Ƿ���������
// 3. GameThreadCallback, ���ڹ�����Ϸ����, ��Ϸʣ��ʱ���

DWORD WINAPI KeepThreadCallback(LPVOID parameter)
{
	//CGameIocp *Owner = (CGameIocp*) parameter;
	//Owner->KeepThreadCallback();

	return 0;
}

DWORD WINAPI GameThreadCallback(LPVOID parameter)
{
	CGameIocp *Owner = (CGameIocp*) parameter;
	Owner->GameThreadCallback();

	return 0;
}

DWORD WINAPI SyncThreadCallback(LPVOID parameter)
{
	CGameIocp* Owner = (CGameIocp*)parameter;
	Owner->SyncThreadCallback();

	return 0;
}

CGameIocp::CGameIocp(void)
{
}

CGameIocp::~CGameIocp(void)
{
}

// �����̻߳ص�����
// ÿ�� 30000 ����, ��ͻ��˷��ͼ������, ȷ�Ͽͻ����Ƿ��������ӵ��߳� 
void CGameIocp::KeepThreadCallback(void)
{
	// KeepAlive�ź�ͨ������ֻ����4Byte
	DWORD KeepAlive = 0xFFFF;

	while (true)
	{
		// ÿ30�봫��һ��KeepAlive�ź�
		// �� WaitForSingleObject ������, �� mKeepThreadDestroyEvent ������źŵ�״̬ ( ͨ�� SetEvent(mKeepThreadDestroyEvent) ), ��������״̬
		// �����ڵȴ� 30000 �����, ��������״̬
		DWORD Result = WaitForSingleObject(mKeepThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0) // WaitForSingleObject ��Ϊ���źŵ�״̬, �����߳�
			return;
		// ����ConnectdUserManager�������û�����
		mConnectedUserManager.WriteAll(0x3000000, (BYTE*) &KeepAlive, sizeof(DWORD));
		printf("KeepThreadCallback\n");
	}
}

// ��Ϸģ���о��̵߳Ļص�����
// ����ÿ�� 1000 ������һ����Ϸʱ����߳�
// ��̳��Ը��� CIocp ���е� WorkerThread ���ò�ͬ, WorkerThread ���ڴ�������ͨ�ŵĸ����
// GameThreadCallback ���ڹ�����Ϸ����
void CGameIocp::GameThreadCallback(void)
{
	// NEW_UPDATE
	//return;

	while (true)
	{
		//DWORD Result = WaitForSingleObject(mGameThreadDestroyEvent, 1000);
		DWORD Result = WaitForSingleObject(mGameThreadDestroyEvent, 1000 / 60);

		if (Result == WAIT_OBJECT_0) // WaitForSingleObject ��Ϊ���źŵ�״̬, �����߳�
			return;
		// ÿ�����һ��, ������Ϸ����, �������Ϸ���ж೤ʱ�������
		mRoomManager.UpdateRooms(this);
	}
}

// ��ʱͬ����������ͻ���ս��̬���̵߳Ļص�����
void CGameIocp::SyncThreadCallback(void)
{
	while (true)
	{
		DWORD Result = WaitForSingleObject(mSyncThreadDestroyEvent, 1000);
		
		if (Result == WAIT_OBJECT_0)
			return;
		// �˴�ͨ�� mRoomManager �㲥ս��̬�Ƹ�����Ϣ
		mRoomManager.SyncRooms(this);
	}
}

bool CGameIocp::Begin(void)
{
	// ��ȡ�����������ļ�
	CIniFile	IniFile;

	if (IniFile.Open(CONFIG_FILENAME))
	{
		//IniFile.GetValue(_T("SERVER"), _T("DEFAULT_GAMETIME"), &DEFAULT_GAMETIME);
		IniFile.Close();
	}
	else
	{
		CLog::WriteLogNoDate(_T("IniFile.Open"));
		// û���ҵ��ļ�, ���������� false
		End();
		return false;
	}

	// ��ʼ������ CIocp
	// ���� CPU �������������� workThread
	// ������ɶ˿ڵľ��ֵ mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	// ���� IOCP �Ĺ����߳�, HANDLE WorkerThread = CreateThread(nullptr, 0, ::WorkerThreadCallback, this, 0, nullptr);
	// ͨ�� mWorkerThreadVector ���й���, mWorkerThreadVector.push_back(WorkerThread);
	// �� CIocp::WorkerThreadCallback ��, ͨ������ GetQueuedCompletionStatus �����߳�, �ȴ� Socket IO ���
	// Ȼ����� OnIoRead, OnIoWrite, OnIoConnected, OnIoDisconnected ���д���
	if (!CIocp::Begin()) // ������ WorkerThread, ��������ͨ�� Socket ����¼��Ĵ���
	{	
		CLog::WriteLogNoDate(_T("CIocp::Begin"));
		End();
		return false;
	}

	// CNetworkSession ���� mListenSession ��ʼ��
	// �е�Listen�Ķ���
	if (!mListenSession.Begin())
	{
		CLog::WriteLogNoDate(_T("ListenSession.Begin"));
		End();
		return false;
	}

	// �������� TCP �� ServerSocket
	// �� CNetworkSession ��, mSocket	= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (!mListenSession.TcpBind()){
		CLog::WriteLogNoDate(_T("ListenSession.TcpBind"));
		End();
		return false;
	}

	// �󶨴�����TCP�� ServerSocket ���ڼ����ͻ�������
	// CNetworkSession::Listen(USHORT port, INT backLog), ���� backLog Ϊ�Ѿ�������������ӵ���δ���ӵ��û���󻺳���
	// �� CNetworkSession ��, bind(mSocket, (struct sockaddr*) &ListenSocketInfo, sizeof(SOCKADDR_IN))
	// �˴� MAX_USER Ϊ�ɵȴ����ӵ����������
	if (!mListenSession.Listen(DEFAULT_PORT, MAX_USER))
	{
		CLog::WriteLogNoDate(_T("ListenSession.Listen"));
		End();
		return false;
	}

	// ��ʹ�� Accept ������ʱ��, ͨ�� CreateIoCompletionPort �������ͻ������ӵ� Socket ����ɶ˿ڰ�
	// ��ʹ�� AcceptEx ������ʱ��, ͨ�� CreateIoCompletionPort ������ ListenSocket ( ServerSocket ) ����ɶ˿ڰ�
	// ��ʹ�� AcceptEx ������ʱ��, ����Ҫͨ�� CreateIoCompletionPort �������ͻ������ӵ� Socket ����ɶ˿ڰ�
	// �� CIocp ����, ʹ�õ��� AcceptEx ����, ��˰� mListenSession �� mSocket ( �� �����õ� ServerSocket )
	if (!CIocp::RegisterSocketToIocp(mListenSession.GetSocket(), (ULONG_PTR) &mListenSession))
	{
		CLog::WriteLogNoDate(_T("CIocp::RegisterSocketToIocp"));
		End();
		return false;
	}

	// ���ݿ����ӳ�
	if(!mMySqlSessionPool.Begin())
	{
		CLog::WriteLogNoDate(_T("MySqlSessionPool.Begin"));
		End();
		return false;
	}

	if (!mConnectedUserManager.Begin(MAX_USER, mListenSession.GetSocket()))
	{
		CLog::WriteLogNoDate(_T("ConnectedUserManager.Begin"));
		End();
		return false;
	}

	// ?????????????????????????????
	// mRoomManager ���� Begin ���� MAX_USER �� Room
	if (!mRoomManager.Begin(MAX_USER))
	{
		CLog::WriteLogNoDate(_T("RoomManager.Begin"));
		End();
		return false;
	}
	
	// �� mConnectedUserVector �е����� ConnectedUser �� socket ����Ϊ�ȴ��ͻ������ӵ�״̬ �����socketͶ�� acceptEx ����
	if (!mConnectedUserManager.AcceptAll())
	{
		CLog::WriteLogNoDate(_T("ConnectedUserManager.AllAccept"));
		End();
		return false;
	}

	// ���������߳������¼����
	mKeepThreadDestroyEvent = CreateEvent( // Return Handle �¼����� ��ʱΪ���ź�״̬
		nullptr, // LPSECURITY_ATTRIBUTES  lpEventAttributes ��ȫ����
		false, // bool  bManualReset ��λ��ʽ
		false, // bool  bInitialState ��ʼ״̬
		nullptr); //  LPCTSTR  lpName ��������
	if (!mKeepThreadDestroyEvent)
	{
		End();
		return false;
	}

	// ���������߳̾��
	mKeepThreadHandle		= CreateThread(
		nullptr, // LPSECURITY_ATTRIBUTES  lpThreadAttributes ��ȫ����
		0, // SIZE_T  dwStackSize ���ó�ʼջ��С, Ϊ 0, Ĭ��ʹ������øú����߳���ͬ��ջ�ռ��С
		::KeepThreadCallback, // LPTHRREAD_START_ROUTINE  lpStartAddress // ָ���̺߳�����ָ��
		this, // LPvoid  lpParameter ����Ĳ���
		0, // DWORD  dwCreationFlags
		nullptr); // LPDWORD  lpThreadId
	if (!mKeepThreadHandle)
	{
		End();
		return false;
	}

	// ������Ϸģ���о������߳����پ��
	mGameThreadDestroyEvent = CreateEvent(nullptr, false, false, nullptr); // �����¼�, ��ʱΪ���ź�״̬
	if (!mGameThreadDestroyEvent)
	{
		End();
		return false;
	}
	// ������Ϸģ���о������߳̾��
	mGameThreadHandle		= CreateThread(nullptr, 0, ::GameThreadCallback, this, 0, nullptr);
	if (!mGameThreadHandle)
	{
		End();
		return false;
	}

	// ������Ϸģ���о������߳����پ��
	mSyncThreadDestroyEvent = CreateEvent(nullptr, false, false, nullptr); // �����¼�, ��ʱΪ���ź�״̬
	if (!mGameThreadDestroyEvent)
	{
		End();
		return false;
	}
	// ������Ϸģ���о������߳̾��
	mSyncThreadHandle = CreateThread(nullptr, 0, ::SyncThreadCallback, this, 0, nullptr);
	if (!mGameThreadHandle)
	{
		End();
		return false;
	}

	return true;
}

bool CGameIocp::End(void)
{
	// ����KeepAliveThread
	if (mKeepThreadDestroyEvent && mKeepThreadHandle)
	{
		SetEvent(mKeepThreadDestroyEvent); // ���� mKeepThreadDestroyEvent Ϊ���ź�, ���� KeepThreadCallback

		// KeepThreadHandle ������, ��Ϊ���źŵ�״̬, ��������״̬
		WaitForSingleObject(mKeepThreadHandle, INFINITE); 

		CloseHandle(mKeepThreadDestroyEvent);
		CloseHandle(mKeepThreadHandle);
	}

	// ���� GameThread
	if (mGameThreadDestroyEvent && mGameThreadHandle)
	{
		SetEvent(mGameThreadDestroyEvent); // ���� mGameThreadDestroyEvent Ϊ���ź�, ���� GameThreadCallback

		// mGameThreadHandle ������, ��Ϊ���źŵ�״̬, ��������״̬
		WaitForSingleObject(mGameThreadHandle, INFINITE);

		CloseHandle(mGameThreadDestroyEvent);
		CloseHandle(mGameThreadHandle);
	}

	// ���� SyncThread
	if (mSyncThreadDestroyEvent && mSyncThreadHandle)
	{
		SetEvent(mSyncThreadDestroyEvent);
		WaitForSingleObject(mSyncThreadHandle, INFINITE);

		CloseHandle(mSyncThreadDestroyEvent);
		CloseHandle(mSyncThreadHandle);
	}

	CIocp::End();

	mRoomManager.End();

	mConnectedUserManager.End();

	mListenSession.End();

	// ���������ݿ�
	//mDataBase.End();
	mMySqlSessionPool.End();

	return true;
}




























