#pragma once
#include "ConnectedUser.h"
#include "BattleField.h"
#include "GameCtrlParams.h"
//class CConnectedUser;
class CGameIocp;

using namespace std;

typedef enum ROOM_STATUS
{
	ROOM_STATUS_EMPTY = 0,
	ROOM_STATUS_WAITING,
	ROOM_STATUS_READY,
	ROOM_STATUS_GAME_START,
	ROOM_STATUS_GAME_IN_PROGRESS,
	ROOM_STATUS_GAME_ENDING,
	ROOM_STATUS_GAME_ENDED
};

typedef enum ROOM_TYPE
{
	RM_NO_TYPE = 0,
	RM_TYPE_1V1,
	RM_TYPE_2V2
};

class CRoom : public CMultiThreadSync<CRoom>
{
public:
	CRoom(void);
	virtual ~CRoom(void);

private:
	ROOM_TYPE							mType;
	DWORD										mIndex; // 房间编号

	ROOM_STATUS						mStatus;
	bool											mIsGameStarted; // 确认是否开始游戏的变量
	
	//list<CConnectedUser*> mUserList;
	list<CConnectedUser*> mUsersInBlueTeam;
	list<CConnectedUser*> mUsersInRedTeam;

	// 战场
	CBattleField* mBattleField;

	//////////////////////////////////////////////////////////////////////////
	// 游戏相关变量


	//////////////////////////////////////////////////////////////////////////

private:
	// 在屏幕上打印传输协议
	void printProtocol(int protocol);

public:
	bool					Begin(DWORD index);
	bool					End(void);

	// 战场 BattleField 的存取函数
	void SetBattleField(CBattleField* battleField) { mBattleField = battleField; };
	CBattleField* GetBattleField() { return mBattleField; };


	// 用户进入房间 或 离开房间时调用的函数
	bool					JoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType);
	bool					LeaveRoom(bool isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser);


	// 设置房间中所有玩家状态
	void					SetUserStatusAll(ENUM_USER_STATUS status);
	// 向房间内所有用户传送数据包时使用的函数
	bool					WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);

	// 房主按下开始游戏按钮时运行的函数
	bool					GameStart(void);
	bool					GameEnd(CGameIocp *iocp);

	// 检查游戏房间是否有玩家存在
	inline bool				GetIsEmpty(void){CThreadSync Sync;return (mUsersInBlueTeam.size() + mUsersInRedTeam.size()) ? false : true;}

	// 检查游戏房间是否玩家已满
	inline bool				GetIsFull(int maxUserNum){CThreadSync Sync;return (mUsersInBlueTeam.size() + mUsersInRedTeam.size()) == maxUserNum ? true : false;}

	// 检查游戏房间当时是否已经开始游戏
	inline bool				GetIsGameStarted(void){CThreadSync Synd;return mIsGameStarted;}

	// 获取当前房间编号
	inline DWORD			GetIndex(void){CThreadSync Sync;return mIndex;}

	// 获取当前房间在线用户数
	inline USHORT			GetCurrentUserCount(void){CThreadSync Sync;return (mUsersInBlueTeam.size() + mUsersInRedTeam.size());}

	inline void SetType(ROOM_TYPE type){CThreadSync Sync;mType = type;}
	inline ROOM_TYPE GetType(){ CThreadSync Sync; return mType; }

	inline void SetStatus(ROOM_STATUS status){CThreadSync Sync;mStatus = status;};
	inline ROOM_STATUS GetStatus(){ CThreadSync Sync; return mStatus; };

	//////////////////////////////////////////////////////////////////////////
	// 游戏管理函数
	void BroadcastBattleSituation();
	ROOM_STATUS UpdateGameProcess();
	//////////////////////////////////////////////////////////////////////////
};
