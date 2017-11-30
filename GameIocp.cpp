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

// 在 GameIocp 类中,相当于开启了 3 个线程
// 1. 父类 CIocp 中的 WorkerThread, 用于管理 IOCP 的网络通信
// 2. KeepThreadCallback, 用于检测客户端是否连接正常
// 3. GameThreadCallback, 用于管理游戏规则, 游戏剩余时间等

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

CGameIocp::CGameIocp(VOID)
{
}

CGameIocp::~CGameIocp(VOID)
{
}

// 每隔 30000 毫秒, 向客户端发送检测数据, 确认客户端是否正常连接的线程 
VOID CGameIocp::KeepThreadCallback(VOID)
{
	// KeepAlive信号通过数据只发送4Byte
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		// 每30秒传送一次KeepAlive信号
		// 在 WaitForSingleObject 函数中, 当 mKeepThreadDestroyEvent 变成有信号的状态 ( 通过 SetEvent(mKeepThreadDestroyEvent) ), 结束阻塞状态
		// 或者在等待 30000 毫秒后, 结束阻塞状态
		DWORD Result = WaitForSingleObject(mKeepThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0) // WaitForSingleObject 变为有信号的状态, 结束线程
			return;

		// 利用ConnectdUserManager向所有用户传递
		mConnectedUserManager.WriteAll(0x3000000, (BYTE*) &KeepAlive, sizeof(DWORD));
		printf("KeepThreadCallback\n");
	}
}

// 用于每隔 1000 毫秒检测一次游戏时间的线程
// 与继承自父类 CIocp 类中的 WorkerThread 作用不同, WorkerThread 用于处理网络通信的各项工作
// GameThreadCallback 用于管理游戏规则
VOID CGameIocp::GameThreadCallback(VOID)
{
	// NEW_UPDATE
	//return;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mGameThreadDestroyEvent, 1000);

		if (Result == WAIT_OBJECT_0) // WaitForSingleObject 变为有信号的状态, 结束线程
			return;

		// 每秒调用一次, 处理游戏规则, 如计算游戏还有多长时间结束等
		mRoomManager.UpdateRooms(this);
	}
}

BOOL CGameIocp::Begin(VOID)
{
	// 读取并保存数据文件
	CIniFile	IniFile;

	if (IniFile.Open(CONFIG_FILENAME))
	{
		//IniFile.GetValue(_T("SERVER"), _T("DEFAULT_GAMETIME"), &DEFAULT_GAMETIME);
		IniFile.Close();
	}
	else
	{
		CLog::WriteLogNoDate(_T("IniFile.Open"));

		// 没有找到文件, 结束并返回 FALSE
		End();

		return FALSE;
	}

	// 初始化父类 CIocp
	// 根据 CPU 核数生成两倍的 workThread
	// 创建完成端口的句柄值 mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	// 创建 IOCP 的工作线程, HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
	// 通过 mWorkerThreadVector 进行管理, mWorkerThreadVector.push_back(WorkerThread);
	// 在 CIocp::WorkerThreadCallback 中, 通过调用 GetQueuedCompletionStatus 阻塞线程, 等待 Socket IO 完成
	// 然后调用 OnIoRead, OnIoWrite, OnIoConnected, OnIoDisconnected 进行处理
	if (!CIocp::Begin()) // 启动了 WorkerThread, 用于网络通信 Socket 完成事件的处理
	{	
		CLog::WriteLogNoDate(_T("CIocp::Begin"));

		End();

		return FALSE;
	}

	// CNetworkSession 对象 mListenSession 初始化
	// 承担Listen的对象
	if (!mListenSession.Begin())
	{
		CLog::WriteLogNoDate(_T("ListenSession.Begin"));

		End();

		return FALSE;
	}

	// 创建用于 TCP 的 ServerSocket
	// 在 CNetworkSession 中, mSocket	= WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (!mListenSession.TcpBind()){
		CLog::WriteLogNoDate(_T("ListenSession.TcpBind"));

		End();

		return FALSE;
	}

	// 绑定创建的TCP的 ServerSocket 用于监听客户端请求
	// CNetworkSession::Listen(USHORT port, INT backLog), 其中 backLog 为已经请求服务器连接但尚未连接的用户最大缓冲数
	// 在 CNetworkSession 中, bind(mSocket, (struct sockaddr*) &ListenSocketInfo, sizeof(SOCKADDR_IN))
	// 此处 MAX_USER 为可等待连接的最大请求数
	if (!mListenSession.Listen(DEFAULT_PORT, MAX_USER))
	{
		CLog::WriteLogNoDate(_T("ListenSession.Listen"));

		End();

		return FALSE;
	}

	// 在使用 Accept 函数的时候, 通过 CreateIoCompletionPort 函数将客户端连接的 Socket 与完成端口绑定
	// 在使用 AcceptEx 函数的时候, 通过 CreateIoCompletionPort 函数将 ListenSocket ( ServerSocket ) 与完成端口绑定
	// 在使用 AcceptEx 函数的时候, 还需要通过 CreateIoCompletionPort 函数将客户端连接的 Socket 与完成端口绑定
	// 在 CIocp 类中, 使用的是 AcceptEx 函数, 因此绑定 mListenSession 的 mSocket ( 即 监听用的 ServerSocket )
	if (!CIocp::RegisterSocketToIocp(mListenSession.GetSocket(), (ULONG_PTR) &mListenSession))
	{
		CLog::WriteLogNoDate(_T("CIocp::RegisterSocketToIocp"));

		End();

		return FALSE;
	}

	// 数据库连接池
	if(!mMySqlSessionPool.Begin())
	{
		CLog::WriteLogNoDate(_T("MySqlSessionPool.Begin"));

		End();

		return FALSE;
	}

	if (!mConnectedUserManager.Begin(MAX_USER, mListenSession.GetSocket()))
	{
		CLog::WriteLogNoDate(_T("ConnectedUserManager.Begin"));

		End();

		return FALSE;
	}

	// ?????????????????????????????
	// mRoomManager 调用 Begin 生成 MAX_USER 个 Room
	if (!mRoomManager.Begin(MAX_USER))
	{
		CLog::WriteLogNoDate(_T("RoomManager.Begin"));

		End();

		return FALSE;
	}
	
	// 将 mConnectedUserVector 中的所有 ConnectedUser 的 socket 设置为等待客户端连接的状态 向监听socket投递 acceptEx 请求
	if (!mConnectedUserManager.AcceptAll())
	{
		CLog::WriteLogNoDate(_T("ConnectedUserManager.AllAccept"));

		End();

		return FALSE;
	}

	mKeepThreadDestroyEvent = CreateEvent( // Return Handle 事件对象 此时为无信号状态
		NULL, // LPSECURITY_ATTRIBUTES  lpEventAttributes 安全属性
		FALSE, // BOOL  bManualReset 复位方式
		FALSE, // BOOL  bInitialState 初始状态
		NULL); //  LPCTSTR  lpName 对象名称
	if (!mKeepThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	// 处理KeepAliveThread相关操作
	mKeepThreadHandle		= CreateThread(
		NULL, // LPSECURITY_ATTRIBUTES  lpThreadAttributes 安全属性
		0, // SIZE_T  dwStackSize 设置初始栈大小, 为 0, 默认使用与调用该函数线程相同的栈空间大小
		::KeepThreadCallback, // LPTHRREAD_START_ROUTINE  lpStartAddress // 指向线程函数的指针
		this, // LPVOID  lpParameter 传入的参数
		0, // DWORD  dwCreationFlags
		NULL); // LPDWORD  lpThreadId
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}

	mGameThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 创建事件, 此时为无信号状态
	if (!mGameThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mGameThreadHandle		= CreateThread(NULL, 0, ::GameThreadCallback, this, 0, NULL);
	if (!mGameThreadHandle)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CGameIocp::End(VOID)
{
	// 结束KeepAliveThread和GameThread
	if (mKeepThreadDestroyEvent && mKeepThreadHandle)
	{
		SetEvent(mKeepThreadDestroyEvent); // 设置 mKeepThreadDestroyEvent 为有信号, 结束 KeepThreadCallback

		// KeepThreadHandle 结束后, 变为有信号的状态, 结束阻塞状态
		WaitForSingleObject(mKeepThreadHandle, INFINITE); 

		CloseHandle(mKeepThreadDestroyEvent);
		CloseHandle(mKeepThreadHandle);
	}

	if (mGameThreadDestroyEvent && mGameThreadHandle)
	{
		SetEvent(mGameThreadDestroyEvent); // 设置 mGameThreadDestroyEvent 为有信号, 结束 GameThreadCallback

		// mGameThreadHandle 结束后, 变为有信号的状态, 结束阻塞状态
		WaitForSingleObject(mGameThreadHandle, INFINITE);

		CloseHandle(mGameThreadDestroyEvent);
		CloseHandle(mGameThreadHandle);
	}

	CIocp::End();

	mRoomManager.End();

	mConnectedUserManager.End();

	mListenSession.End();

	// 最后结束数据库
	//mDataBase.End();
	mMySqlSessionPool.End();

	return TRUE;
}




























