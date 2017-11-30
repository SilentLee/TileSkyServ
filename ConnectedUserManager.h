// 以向量的方式管理ConnectedUser
// 在多线程环境中, 想要以Vector形态管理对象的话, 需要线程同步对象
#pragma once

class CConnectedUserManager : public CMultiThreadSync<CConnectedUserManager>
{
public:
	CConnectedUserManager(VOID);
	virtual ~CConnectedUserManager(VOID);

private:
	// 用于存储 ConnectedUser 的向量
	std::vector<CConnectedUser*>	mConnectedUserVector;

	// 可存储的最大用户数量
	DWORD							mMaxUserCount;

	// 当前连接的用户数量, 并存储在 mConnectedUserVector 中的用户数量
	DWORD							mOnlineUserCount;
	
	// 用于监听的 Socket
	SOCKET							mListenSocket;

public:
	BOOL	Begin(DWORD maxUserCount, SOCKET listenSocket);
	BOOL	End(VOID);

	// 设置所有的 ConnectedUser 处于准备接受连接的状态
	BOOL	AcceptAll(VOID);

	// 向 所有的 ConnectedUer 统一发送数据
	BOOL	WriteAll(DWORD protocol, BYTE *data, DWORD dataLength);

	// 检查 ConnectedUser 是否已经登录
	BOOL	IsAlreadyLogined(LPTSTR userID);

	// mCurrentUserCount 自加 1, InterLockedIncrement 函数为实现传入指针中存储的数值实现原子操作的加 1
	inline VOID	IncreaseConnectedUserCount(VOID){InterlockedIncrement((LONG*) &mOnlineUserCount);}

	// mCurrentUserCount 自减 1, InterlockedDecrement 函数为实现传入指针中存储的数值实现原子操作的减 1
	inline VOID	DecreaseConnectedUserCount(VOID){InterlockedDecrement((LONG*) &mOnlineUserCount);}
};
