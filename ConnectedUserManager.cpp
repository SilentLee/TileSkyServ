#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Room.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"

CConnectedUserManager::CConnectedUserManager(VOID)
{
	mMaxUserCount		= 0;
}

CConnectedUserManager::~CConnectedUserManager(VOID)
{
}

// 初始化 ConnectedUserManager
BOOL CConnectedUserManager::Begin(DWORD maxUserCount, SOCKET listenSocket)
{
	CThreadSync Sync;

	if (maxUserCount <= 0 || !listenSocket)
		return FALSE;

	mMaxUserCount		= maxUserCount;

	mListenSocket		= listenSocket;

	// 批量生成 ConnectedUser, 并存入 mConnectedUserVector 中
	for (DWORD i=0;i<mMaxUserCount;i++)
	{
		CConnectedUser *ConnectedUser = new CConnectedUser();

		// 初始化 ConnectedUser
		if (ConnectedUser->Begin())
			mConnectedUserVector.push_back(ConnectedUser);
		else
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

// 结束 ConnectedUserManager
BOOL CConnectedUserManager::End(VOID)
{
	CThreadSync Sync;

	// 遍历 mConnectorUserVector
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		// 新生成一个指针用于引用 mConnectorUserVector 中的元素
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// 结束每一个 ConnectedUser
		ConnectedUser->End();

		// 通过删除 ConnectedUser 指针, 删除对应的 mConnectedUserVector[i] 指向的内存
		delete ConnectedUser;
	}

	// mConnectedUserVector 清零
	// 清除mConnectedUserVector 中仍然保存的内存地址 
	// 但此时内存地址所指向的内存已经被释放
	mConnectedUserVector.clear();

	return TRUE;
}

// 将所有的 mConnectedUser 设为等待接收状态
BOOL CConnectedUserManager::AcceptAll(VOID)
{
	CThreadSync Sync;

	// 遍历 mConnectedUserVector 中的 ConnectedUser
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// 将 ConnectedUser 设置为准备接收客户端连接的状态
		if (!ConnectedUser->Accept(mListenSocket))
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

// 向所有的在线用户发送消息
BOOL CConnectedUserManager::WriteAll(DWORD protocol, BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!data)
		return FALSE;

	// 遍历 mConnectedUserVector 中的 ConnectedUser
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// 如果当前 ConnectedUser 仍然保持连接, 发送数据
		// 如果没有保持连接. 结束当前的 ConnectedUser
		if (ConnectedUser->GetIsConnected())
		{
			if (!ConnectedUser->WritePacket(protocol, data, dataLength))
				ConnectedUser->End();
		}
	}

	return TRUE;
}

// 在 mConnectedUserVector 中查找 userID, 若存在则登录成功
BOOL CConnectedUserManager::IsAlreadyLogined(LPTSTR userID)
{
	CThreadSync Sync;

	if (!userID)
		return TRUE;

	//TCHAR UserID[32] = {0,};

	// 在 mConnectedUserVector 中查找 userID
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// 将传入的 userID 与当前的 ConnectedUser 中的 UserID 进行比较
		/*if (!_tcscmp(ConnectedUser->GetUserID(), userID))
			return TRUE;*/
	}

	return FALSE;
}