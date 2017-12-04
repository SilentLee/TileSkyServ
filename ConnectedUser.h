// �����û���Ϣ����
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

// �˴��򵥶���һ�� CRoom ��, �Ƿ�����ڲ����� CRoom ��ͷ�ļ��������, ��Ӱ���ļ��� CRoom �������, 
// ���ڱ����ʱ��, ֱ���ҵ������ļ��� CRoom ��Ķ���
// �����������ľ�������, ֻ���ڵ�ǰ�ĵ���Χ��ʹ�� CRoom ��ĵ�ַ, �����ܵ��� CRoom �����еı����뺯��
class CRoom;

class CConnectedUser : public CPacketSession
{
public:
	CConnectedUser(void);
	virtual ~CConnectedUser(void);

private:
	S_USER_ACCOUNT_INFO mUserAccountInfo; // �û�������Ϣ
	S_USER_EQUIPMENT_INFO mUserEquipmentInfo; // �û�װ����Ϣ
	S_USER_CHEST_INFO mUserChestInfo; // �û���������Ϣ
	
	TCHAR				mVirtualAddress[32]; // �û���������ַ
	TCHAR				mRealAddress[32]; // �û�������ַ
	USHORT			mVirtualPort; // �û��������˿�
	USHORT			mRealPort; // �û������˿�

	// �û���ǰ״̬ US_NONE, US_CHANNEL_ENTERING, US_CHANNEL_ENTERED, 
	// �û���ǰ״̬ US_ROOM_ENTERING, US_ROOM_ENTERED, US_ROOM_LEAVING, US_GAME_STARTING,
	// �û���ǰ״̬ US_GAME_STARTED, US_GAME_ENDING
	int				mStatus; // �û���ǰ״̬
	CRoom*	mEnteredRoom; // �û���ǰ��¼����ĵ�ַ
	bool		mIsConnected; // �Ƿ����ӱ�־λ

	int mNumShotDown; // ��һ���ɻ���


private:
	 // ����Ϸ��������һ��
	ENUM_SIDE_IN_GAME mSideInGame;



	CConnectedUser* mOppoUser;
public:
	bool		Begin(void);
	bool		End(void);
	bool		GameEnd(void);

	bool		Reload(SOCKET listenSocket); // ��¼����ʱ���״̬�����¸���ΪAccept״̬�ĺ���

public:
	// �����ݿ�װ���û���Ϣ�ĺ���
	void		loadUserInfoFromDB(void* userInfo);
	void		loadUserEquipmentInfoWithDefaultValue();
	void		loadUserChestInfoWithDefaultValue() {CThreadSync Sync; memset(&mUserChestInfo, 0, sizeof(S_USER_CHEST_INFO));};

	// ���ݿ��������
	INT			db_VisitorRegist(CMySqlSessionPool mySqlSessionPool);
	INT			db_UserNameRegist(CMySqlSessionPool mySqlSessionPool, S_PT_USER_NAME_REGIST ptUserNameRegist);
	INT			db_BindAccountRegist(CMySqlSessionPool mySqlSessionPool, S_PT_BIND_ACCOUNT_REGIST ptBindAccountRegist);
	INT			db_CellphoneNoRegist(CMySqlSessionPool mySqlSessionPool, S_PT_CELLPHONE_NO_REGIST ptCellphoneNoRegist);
	S_PT_USER_NAME_LOGIN_SUCC_U* db_UserNameLogin(CMySqlSessionPool mySqlSessionPool, S_PT_USER_NAME_LOGIN ptUserNameLogin);
	S_PT_BIND_ACCOUNT_LOGIN_SUCC_U* db_BindAccountLogin(CMySqlSessionPool mySqlSessionPool, S_PT_BIND_ACCOUNT_LOGIN ptBindAccountLogin);
	S_PT_CELLPHONE_NO_LOGIN_SUCC_U* db_CellphoneNoLogin(CMySqlSessionPool mySqlSessionPool, S_PT_CELLPHONE_NO_LOGIN ptCellphoneNoLogin);
	S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U* db_VisitorDefaultLogin(CMySqlSessionPool mySqlSessionPool, S_PT_VISITOR_DEFAULT_LOGIN ptVisitorDefaultLogin);
	bool db_UpdateUserAccountInfo(CMySqlSessionPool mySqlSessionPool);

	// ������ȡ����
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

	// ����ɻ���
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
	// ��Ϸ��غ���
	// Ϊ�˿�ʼ��Ϸ����ʼ����Ϸ��Ϣ
	bool InitializeForGameStart();
};
