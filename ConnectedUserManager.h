// �������ķ�ʽ����ConnectedUser
// �ڶ��̻߳�����, ��Ҫ��Vector��̬�������Ļ�, ��Ҫ�߳�ͬ������
#pragma once

class CConnectedUserManager : public CMultiThreadSync<CConnectedUserManager>
{
public:
	CConnectedUserManager(VOID);
	virtual ~CConnectedUserManager(VOID);

private:
	// ���ڴ洢 ConnectedUser ������
	std::vector<CConnectedUser*>	mConnectedUserVector;

	// �ɴ洢������û�����
	DWORD							mMaxUserCount;

	// ��ǰ���ӵ��û�����, ���洢�� mConnectedUserVector �е��û�����
	DWORD							mOnlineUserCount;
	
	// ���ڼ����� Socket
	SOCKET							mListenSocket;

public:
	BOOL	Begin(DWORD maxUserCount, SOCKET listenSocket);
	BOOL	End(VOID);

	// �������е� ConnectedUser ����׼���������ӵ�״̬
	BOOL	AcceptAll(VOID);

	// �� ���е� ConnectedUer ͳһ��������
	BOOL	WriteAll(DWORD protocol, BYTE *data, DWORD dataLength);

	// ��� ConnectedUser �Ƿ��Ѿ���¼
	BOOL	IsAlreadyLogined(LPTSTR userID);

	// mCurrentUserCount �Լ� 1, InterLockedIncrement ����Ϊʵ�ִ���ָ���д洢����ֵʵ��ԭ�Ӳ����ļ� 1
	inline VOID	IncreaseConnectedUserCount(VOID){InterlockedIncrement((LONG*) &mOnlineUserCount);}

	// mCurrentUserCount �Լ� 1, InterlockedDecrement ����Ϊʵ�ִ���ָ���д洢����ֵʵ��ԭ�Ӳ����ļ� 1
	inline VOID	DecreaseConnectedUserCount(VOID){InterlockedDecrement((LONG*) &mOnlineUserCount);}
};
