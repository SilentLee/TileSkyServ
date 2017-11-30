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

// ��ʼ�� ConnectedUserManager
BOOL CConnectedUserManager::Begin(DWORD maxUserCount, SOCKET listenSocket)
{
	CThreadSync Sync;

	if (maxUserCount <= 0 || !listenSocket)
		return FALSE;

	mMaxUserCount		= maxUserCount;

	mListenSocket		= listenSocket;

	// �������� ConnectedUser, ������ mConnectedUserVector ��
	for (DWORD i=0;i<mMaxUserCount;i++)
	{
		CConnectedUser *ConnectedUser = new CConnectedUser();

		// ��ʼ�� ConnectedUser
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

// ���� ConnectedUserManager
BOOL CConnectedUserManager::End(VOID)
{
	CThreadSync Sync;

	// ���� mConnectorUserVector
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		// ������һ��ָ���������� mConnectorUserVector �е�Ԫ��
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// ����ÿһ�� ConnectedUser
		ConnectedUser->End();

		// ͨ��ɾ�� ConnectedUser ָ��, ɾ����Ӧ�� mConnectedUserVector[i] ָ����ڴ�
		delete ConnectedUser;
	}

	// mConnectedUserVector ����
	// ���mConnectedUserVector ����Ȼ������ڴ��ַ 
	// ����ʱ�ڴ��ַ��ָ����ڴ��Ѿ����ͷ�
	mConnectedUserVector.clear();

	return TRUE;
}

// �����е� mConnectedUser ��Ϊ�ȴ�����״̬
BOOL CConnectedUserManager::AcceptAll(VOID)
{
	CThreadSync Sync;

	// ���� mConnectedUserVector �е� ConnectedUser
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// �� ConnectedUser ����Ϊ׼�����տͻ������ӵ�״̬
		if (!ConnectedUser->Accept(mListenSocket))
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

// �����е������û�������Ϣ
BOOL CConnectedUserManager::WriteAll(DWORD protocol, BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!data)
		return FALSE;

	// ���� mConnectedUserVector �е� ConnectedUser
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// �����ǰ ConnectedUser ��Ȼ��������, ��������
		// ���û�б�������. ������ǰ�� ConnectedUser
		if (ConnectedUser->GetIsConnected())
		{
			if (!ConnectedUser->WritePacket(protocol, data, dataLength))
				ConnectedUser->End();
		}
	}

	return TRUE;
}

// �� mConnectedUserVector �в��� userID, ���������¼�ɹ�
BOOL CConnectedUserManager::IsAlreadyLogined(LPTSTR userID)
{
	CThreadSync Sync;

	if (!userID)
		return TRUE;

	//TCHAR UserID[32] = {0,};

	// �� mConnectedUserVector �в��� userID
	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		// ������� userID �뵱ǰ�� ConnectedUser �е� UserID ���бȽ�
		/*if (!_tcscmp(ConnectedUser->GetUserID(), userID))
			return TRUE;*/
	}

	return FALSE;
}