// 管理用户信息的类
#pragma once
#include "GameCtrlParams.h"
#include "MySqlSessionPool.h"
#include "MySqlSession.h"
#include "MySqlResReader.h"
#include "USER_PT_Structure.h"
#include "PURCHASE_PT_Structure.h"
#include "PacketSession.h"
#include "UserStruct.h"
//#include "Room.h"

typedef enum ENUM_USER_STATUS
{
	US_NONE = 0,
	US_CHANNEL_ENTERING,
	US_CHANNEL_ENTERED,
	US_ROOM_ENTERING,
	US_ROOM_ENTERED,
	US_GAME_START,
	US_GAME_IN_PROGRESS,
	US_GAME_ENDING,
	US_ROOM_LEAVING,
	AI_GAME_START,
	AI_GAME_IN_PROGRESS,
	AI_GAME_ENDING
};

typedef enum ENUM_SIDE_IN_GAME {
	SIDE_NONE,
	SIDE_BLUE,
	SIDE_RED
};

// 此处简单定义一个 CRoom 类, 是否可以在不引用 CRoom 类头文件的情况下, 不影响文件中 CRoom 类的声明, 
// 而在编译的时候, 直接找到其他文件中 CRoom 类的定义
// 但这种做法的局限在于, 只能在当前文档范围中使用 CRoom 类的地址, 而不能调用 CRoom 类内中的变量与函数
class CRoom;

class CConnectedUser : public CPacketSession
{
public:
	CConnectedUser(void);
	virtual ~CConnectedUser(void);

private:
	S_USER_ACCOUNT_INFO mUserAccountInfo; // 用户基本信息
	S_USER_EQUIPMENT_INFO mUserEquipmentInfo; // 用户装备信息
	S_USER_CHEST_INFO mUserChestInfo; // 用户补给箱信息
	
	TCHAR				mVirtualAddress[32]; // 用户局域网地址
	TCHAR				mRealAddress[32]; // 用户公网地址
	USHORT			mVirtualPort; // 用户局域网端口
	USHORT			mRealPort; // 用户公网端口

	// 用户当前状态 US_NONE, US_CHANNEL_ENTERING, US_CHANNEL_ENTERED, 
	// 用户当前状态 US_ROOM_ENTERING, US_ROOM_ENTERED, US_ROOM_LEAVING, US_GAME_STARTING,
	// 用户当前状态 US_GAME_STARTED, US_GAME_ENDING
	int				mStatus; // 用户当前状态
	CRoom*	mEnteredRoom; // 用户当前登录房间的地址
	bool		mIsConnected; // 是否连接标志位

	int mNumShotDown; // 玩家击落飞机数


private:
	 // 在游戏中属于哪一方
	ENUM_SIDE_IN_GAME mSideInGame;



	CConnectedUser* mOppoUser;
public:
	bool		Begin(void);
	bool		End(void);
	bool		GameEnd(void);

	bool		Reload(SOCKET listenSocket); // 登录结束时清除状态及重新更改为Accept状态的函数

public:
	// 从数据库装载用户信息的函数
	void		loadUserInfoFromDB(void* userInfo);
	void		loadUserEquipmentInfoWithDefaultValue();
	void		loadUserChestInfoWithDefaultValue() {CThreadSync Sync; memset(&mUserChestInfo, 0, sizeof(S_USER_CHEST_INFO));};

	// 数据库操作函数
	INT			db_VisitorRegist(CMySqlSessionPool mySqlSessionPool);
	INT			db_UserNameRegist(CMySqlSessionPool mySqlSessionPool, S_PT_USER_NAME_REGIST ptUserNameRegist);
	INT			db_BindAccountRegist(CMySqlSessionPool mySqlSessionPool, S_PT_BIND_ACCOUNT_REGIST ptBindAccountRegist);
	INT			db_CellphoneNoRegist(CMySqlSessionPool mySqlSessionPool, S_PT_CELLPHONE_NO_REGIST ptCellphoneNoRegist);
	S_PT_USER_NAME_LOGIN_SUCC_U* db_UserNameLogin(CMySqlSessionPool mySqlSessionPool, S_PT_USER_NAME_LOGIN ptUserNameLogin);
	S_PT_BIND_ACCOUNT_LOGIN_SUCC_U* db_BindAccountLogin(CMySqlSessionPool mySqlSessionPool, S_PT_BIND_ACCOUNT_LOGIN ptBindAccountLogin);
	S_PT_CELLPHONE_NO_LOGIN_SUCC_U* db_CellphoneNoLogin(CMySqlSessionPool mySqlSessionPool, S_PT_CELLPHONE_NO_LOGIN ptCellphoneNoLogin);
	S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U* db_VisitorDefaultLogin(CMySqlSessionPool mySqlSessionPool, S_PT_VISITOR_DEFAULT_LOGIN ptVisitorDefaultLogin);
	bool db_UpdateUserAccountInfo(CMySqlSessionPool mySqlSessionPool);

	// 变量存取函数
public:
	void SetSideInGame(ENUM_SIDE_IN_GAME sideInGame){ mSideInGame = sideInGame; };
	ENUM_SIDE_IN_GAME GetSideInGame(){ return mSideInGame; };

	inline S_USER_ACCOUNT_INFO* GetUserAccountInfo(){return &mUserAccountInfo;}
	inline S_USER_EQUIPMENT_INFO* GetUserEquipmentInfo(){return &mUserEquipmentInfo;}
	inline S_USER_CHEST_INFO* GetUserChestInfo(){return &mUserChestInfo;}

	//inline void			IncreaseCurrentPower(){CThreadSync Sync; mCurrentPower < MAX_GAME_POWER ? mCurrentPower ++ : mCurrentPower = MAX_GAME_POWER;}
	//inline int				getCurrentPower(){CThreadSync Sync; return mCurrentPower;}

	inline bool		SetIsConnected(bool isConnected){CThreadSync Sync;mIsConnected = isConnected;return true;}
	inline bool		GetIsConnected(void){CThreadSync Sync;return mIsConnected;}

	// 击落飞机数
	inline void SetNumShotDown(int numShotDown){mNumShotDown = numShotDown;}
	inline void IncreaseNumShotDown(){mNumShotDown ++;};
	inline int GetNumShotDown(){return mNumShotDown;}

	inline bool		SetVirtualInformation(LPTSTR virtualAddress, USHORT virtualPort){CThreadSync Sync;if (!virtualAddress || virtualPort <= 0)return false;_tcsncpy(mVirtualAddress, virtualAddress, 32);mVirtualPort = virtualPort;return true;}
	inline bool		SetRealInformation(LPTSTR realAddress, USHORT realPort){CThreadSync Sync;if (!realAddress || realPort <= 0)return false;_tcsncpy(mRealAddress, realAddress, 32);mRealPort = realPort;return true;}
	inline bool		SetStatus(int status){CThreadSync Sync;mStatus = status;return true;}
	inline int				GetStatus(void){CThreadSync Sync;return mStatus;}
	inline bool		SetEnteredRoom(CRoom *room){CThreadSync Sync;mEnteredRoom = room;return true;}
	inline CRoom*	GetEnteredRoom(void){CThreadSync Sync;return mEnteredRoom;}
	inline LPTSTR	GetRealAddress(void){CThreadSync Sync;return mRealAddress;}
	inline LPTSTR	GetVirtualAddress(void){CThreadSync Sync;return mVirtualAddress;}
	inline USHORT	GetRealPort(void){CThreadSync Sync;return mRealPort;}
	inline USHORT	GetVirtualPort(void){CThreadSync Sync;return mVirtualPort;}

	inline void SetOppoUser(CConnectedUser* oppoUser){mOppoUser = oppoUser;};
	inline CConnectedUser* GetOppoUser(){return mOppoUser;}

	//////////////////////////////////////////////////////////////////////////
	// 游戏相关函数
	// 为了开始游戏而初始化游戏信息
	bool InitializeForGameStart();
};
