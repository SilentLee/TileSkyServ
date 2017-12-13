#pragma once

// CRoomManager 继承多线程同步对象
class CRoomManager : public CMultiThreadSync<CRoomManager>
{
public:
	CRoomManager(VOID);
	virtual ~CRoomManager(VOID);

private:
	DWORD				mMaxRoomCount; // 设定最大房间数
	DWORD				mCurrentRoomCount; // 设定当前房间数

	std::vector<CRoom*>	mRoomVectorClassic;	// 保存经典斗场 CRoom 对象指针的矢量值
	std::vector<CRoom*>	mRoomVectorDouble;	// 保存二人死斗场 CRoom 对象指针的矢量值
	std::vector<CRoom*>	mRoomVectorTrible;		// 保存三人乱斗场 CRoom 对象指针的矢量值

public:
	BOOL	Begin(DWORD maxRoomCount); // 开始函数
	BOOL	End(VOID); // 结束函数

	// 刷新房间状态的函数
	BOOL	UpdateRooms(CGameIocp *iocp);
	// 与客户端同步房间状态的函数
	BOOL SyncRooms(CGameIocp *iocp);

	// 使用快速进入房间功能时所调用的函数
	CRoom*	QuickJoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType);
};
