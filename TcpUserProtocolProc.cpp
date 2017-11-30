#include "stdafx.h"

#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "Stream.h"

#include "ErrorCode.h"
#include "UserProtocol.h"
#include "User_PT_Structure.h"
#include "User_PT_ReadPacket.h"
#include "User_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

// 游客模式注册
VOID CGameIocp::onPT_VISITOR_REGIST(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_VISITOR_REGIST_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_VISITOR_REGIST);
	CLog::WriteLog(_T("# Read packet : PT_VISITOR_REGIST")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体
	
	//INT USER_ID = db_VisitorRegist();
	INT USER_ID = connectedUser->db_VisitorRegist(mMySqlSessionPool);

	// 注册时获取免费补给的时间
	INT64 currentTime;
	time((time_t*)&currentTime);

	connectedUser->WritePacket(PT_VISITOR_REGIST_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_VISITOR_REGIST_SUCC_U( WriteBuffer, 
		USER_ID, 
		"Visitor", 
		DEFAULT_USER_PASSWORD, 
		DEFAULT_BIND_ACCOUNT_TYPE, 
		DEFAULT_BIND_ACCOUNT, 
		DEFAULT_CELLPHONE_NO, 
		0, 1, 100, 1000, 0, 0, 0, 0, 0, 0, 0,(DWORD_PTR)(connectedUser),
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,currentTime));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo, WriteBuffer, sizeof(S_USER_ACCOUNT_INFO));
	connectedUser->loadUserEquipmentInfoWithDefaultValue();

	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_USER_NAME_REGIST(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_USER_NAME_REGIST_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_USER_NAME_REGIST);
	CLog::WriteLog(_T("# Read packet : PT_USER_NAME_REGIST")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//INT USER_ID = db_UserNameRegist(Data);
	INT USER_ID = connectedUser->db_UserNameRegist(mMySqlSessionPool, Data);

	// 注册时获取免费补给的时间
	INT64 currentTime;
	time((time_t*)&currentTime);

	connectedUser->WritePacket(PT_USER_NAME_REGIST_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_USER_NAME_REGIST_SUCC_U( WriteBuffer, 
		USER_ID, 
		Data.NICKNAME, 
		Data.PASSWORD, 
		DEFAULT_BIND_ACCOUNT_TYPE, 
		DEFAULT_BIND_ACCOUNT, 
		DEFAULT_CELLPHONE_NO, 
		0, 1, 100, 1000, 0, 0, 0, 0, 0, 0, 0, (DWORD_PTR)(connectedUser),
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,currentTime));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo, WriteBuffer, sizeof(S_USER_ACCOUNT_INFO));
	connectedUser->loadUserEquipmentInfoWithDefaultValue();

	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_BIND_ACCOUNT_REGIST(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_BIND_ACCOUNT_REGIST_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_BIND_ACCOUNT_REGIST);
	CLog::WriteLog(_T("# Read packet : PT_BIND_ACCOUNT_REGIST")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//INT USER_ID = db_BindAccountRegist(Data);
	INT USER_ID = connectedUser->db_BindAccountRegist(mMySqlSessionPool, Data);

	// 注册时获取免费补给的时间
	INT64 currentTime;
	time((time_t*)&currentTime);

	connectedUser->WritePacket(PT_BIND_ACCOUNT_REGIST_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_BIND_ACCOUNT_REGIST_SUCC_U( WriteBuffer, 
		USER_ID, 
		DEFAULT_BIND_ACCOUNT_NICKNAME, 
		DEFAULT_USER_PASSWORD, 
		Data.BIND_ACCOUNT_TYPE, 
		Data.BIND_ACCOUNT, 
		DEFAULT_CELLPHONE_NO, 
		0, 1, 100, 1000, 0, 0, 0, 0, 0, 0, 0, (DWORD_PTR)(connectedUser),
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,currentTime));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo, WriteBuffer, sizeof(S_USER_ACCOUNT_INFO));
	connectedUser->loadUserEquipmentInfoWithDefaultValue();

	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_CELLPHONE_NO_REGIST(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_CELLPHONE_NO_REGIST_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_CELLPHONE_NO_REGIST);
	CLog::WriteLog(_T("# Read packet : PT_CELLPHONE_NO_REGIST")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//INT USER_ID = db_CellphoneNoRegist(Data);
	INT USER_ID = connectedUser->db_CellphoneNoRegist(mMySqlSessionPool, Data);

	// 注册时获取免费补给的时间
	INT64 currentTime;
	time((time_t*)&currentTime);

	connectedUser->WritePacket(PT_CELLPHONE_NO_REGIST_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_CELLPHONE_NO_REGIST_SUCC_U( WriteBuffer, 
		USER_ID, 
		DEFAULT_CELLPHONE_NO_NICKNAME, 
		DEFAULT_USER_PASSWORD, 
		DEFAULT_BIND_ACCOUNT_TYPE, 
		DEFAULT_BIND_ACCOUNT, 
		Data.CELLPHONE_NO, 
		0, 1, 100, 1000, 0, 0, 0, 0, 0, 0, 0, (DWORD_PTR)(connectedUser),
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,currentTime));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo, WriteBuffer, sizeof(S_USER_ACCOUNT_INFO));
	connectedUser->loadUserEquipmentInfoWithDefaultValue();

	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_USER_NAME_LOGIN(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_USER_NAME_LOGIN_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_USER_NAME_LOGIN);
	CLog::WriteLog(_T("# Read packet : PT_USER_NAME_LOGIN")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//S_PT_USER_NAME_LOGIN_SUCC_U *ptUserNameLoginSuccU = db_UserNameLogin(Data);
	S_PT_USER_NAME_LOGIN_SUCC_U *ptUserNameLoginSuccU = connectedUser->db_UserNameLogin(mMySqlSessionPool, Data);

	if(!ptUserNameLoginSuccU)
	{
		connectedUser->WritePacket(PT_USER_NAME_LOGIN_FAIL_U, WriteBuffer, WRITE_PT_USER_NAME_LOGIN_FAIL_U(WriteBuffer, 1));
		return;
	}

	ptUserNameLoginSuccU->SESSION_ID = (DWORD_PTR)(connectedUser);

	connectedUser->WritePacket(PT_USER_NAME_LOGIN_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_USER_NAME_LOGIN_SUCC_U( WriteBuffer, *ptUserNameLoginSuccU));

	connectedUser->loadUserInfoFromDB(ptUserNameLoginSuccU);

	delete ptUserNameLoginSuccU;
	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_BIND_ACCOUNT_LOGIN(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_BIND_ACCOUNT_LOGIN_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_BIND_ACCOUNT_LOGIN);
	CLog::WriteLog(_T("# Read packet : PT_BIND_ACCOUNT_LOGIN")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//S_PT_BIND_ACCOUNT_LOGIN_SUCC_U *ptBindAccountLoginSuccU = db_BindAccountLogin(Data);
	S_PT_BIND_ACCOUNT_LOGIN_SUCC_U *ptBindAccountLoginSuccU = connectedUser->db_BindAccountLogin(mMySqlSessionPool, Data);

	if(!ptBindAccountLoginSuccU)
	{
		connectedUser->WritePacket(PT_BIND_ACCOUNT_LOGIN_FAIL_U, WriteBuffer, WRITE_PT_BIND_ACCOUNT_LOGIN_FAIL_U(WriteBuffer, 1));
		return;
	}

	ptBindAccountLoginSuccU->SESSION_ID = (DWORD_PTR)(connectedUser);

	connectedUser->WritePacket(PT_BIND_ACCOUNT_LOGIN_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_BIND_ACCOUNT_LOGIN_SUCC_U( WriteBuffer, *ptBindAccountLoginSuccU));

	connectedUser->loadUserInfoFromDB(ptBindAccountLoginSuccU);

	delete ptBindAccountLoginSuccU;
	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_CELLPHONE_NO_LOGIN(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_CELLPHONE_NO_LOGIN_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_CELLPHONE_NO_LOGIN);
	CLog::WriteLog(_T("# Read packet : PT_CELLPHONE_NO_LOGIN")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//S_PT_CELLPHONE_NO_LOGIN_SUCC_U *ptCellphoneNoLoginSuccU = db_CellphoneNoLogin(Data);
	S_PT_CELLPHONE_NO_LOGIN_SUCC_U *ptCellphoneNoLoginSuccU = connectedUser->db_CellphoneNoLogin(mMySqlSessionPool, Data);

	if(!ptCellphoneNoLoginSuccU)
	{
		connectedUser->WritePacket(PT_CELLPHONE_NO_LOGIN_FAIL_U, WriteBuffer, WRITE_PT_CELLPHONE_NO_LOGIN_FAIL_U(WriteBuffer, 1));
		return;
	}

	ptCellphoneNoLoginSuccU->SESSION_ID = (DWORD_PTR)(connectedUser);

	connectedUser->WritePacket(PT_CELLPHONE_NO_LOGIN_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_CELLPHONE_NO_LOGIN_SUCC_U( WriteBuffer, *ptCellphoneNoLoginSuccU));

	connectedUser->loadUserInfoFromDB(ptCellphoneNoLoginSuccU);

	delete ptCellphoneNoLoginSuccU;
	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_VISITOR_DEFAULT_LOGIN(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_NONE, PT_VISITOR_DEFAULT_LOGIN_FAIL_U);
	connectedUser->SetStatus(US_CHANNEL_ENTERING);
	READ_PACKET(PT_VISITOR_DEFAULT_LOGIN);
	CLog::WriteLog(_T("# Read packet : PT_VISITOR_DEFAULT_LOGIN")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	//S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U *ptVisitorDefaultLoginSuccU = db_VisitorDefaultLogin(Data);
	S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U *ptVisitorDefaultLoginSuccU = connectedUser->db_VisitorDefaultLogin(mMySqlSessionPool, Data);

	if(!ptVisitorDefaultLoginSuccU)
	{
		connectedUser->WritePacket(PT_VISITOR_DEFAULT_LOGIN_FAIL_U, WriteBuffer, WRITE_PT_VISITOR_DEFAULT_LOGIN_FAIL_U(WriteBuffer, 1));
		return;
	}

	ptVisitorDefaultLoginSuccU->SESSION_ID = (DWORD_PTR)(connectedUser);

	connectedUser->WritePacket(PT_VISITOR_DEFAULT_LOGIN_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_VISITOR_DEFAULT_LOGIN_SUCC_U( WriteBuffer, *ptVisitorDefaultLoginSuccU));

	connectedUser->loadUserInfoFromDB(ptVisitorDefaultLoginSuccU);

	delete ptVisitorDefaultLoginSuccU;
	connectedUser->SetStatus(US_CHANNEL_ENTERED);
}

VOID CGameIocp::onPT_ADD_NICKNAME(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_ADD_NICKNAME_FAIL_U);
	READ_PACKET(PT_ADD_NICKNAME);
	CLog::WriteLog(_T("# Read packet : PT_ADD_NICKNAME")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_ADD_NICKNAME_SUCC_U ptAddNicknameSuccU;
	memcpy(ptAddNicknameSuccU.NICKNAME, Data.NICKNAME, 32);

	connectedUser->WritePacket(PT_ADD_NICKNAME_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_ADD_NICKNAME_SUCC_U( WriteBuffer, ptAddNicknameSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo->NICKNAME, ptAddNicknameSuccU.NICKNAME, 32);
}

VOID CGameIocp::onPT_ADD_BIND_ACCOUNT(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_ADD_BIND_ACCOUNT_FAIL_U);
	READ_PACKET(PT_ADD_BIND_ACCOUNT);
	CLog::WriteLog(_T("# Read packet : PT_ADD_BIND_ACCOUNT")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_ADD_BIND_ACCOUNT_SUCC_U ptAddBindAccountSuccU;
	memcpy(ptAddBindAccountSuccU.BIND_ACCOUNT_TYPE, Data.BIND_ACCOUNT_TYPE, 32);
	memcpy(ptAddBindAccountSuccU.BIND_ACCOUNT, Data.BIND_ACCOUNT, 32);

	connectedUser->WritePacket(PT_ADD_BIND_ACCOUNT_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_ADD_BIND_ACCOUNT_SUCC_U( WriteBuffer, ptAddBindAccountSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo->BIND_ACCOUNT_TYPE, ptAddBindAccountSuccU.BIND_ACCOUNT_TYPE, 32);
	memcpy(userAccountInfo->BIND_ACCOUNT, ptAddBindAccountSuccU.BIND_ACCOUNT, 32);
}

VOID CGameIocp::onPT_ADD_CELLPHONE_NO(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_ADD_CELLPHONE_NO_FAIL_U);
	READ_PACKET(PT_ADD_CELLPHONE_NO);
	CLog::WriteLog(_T("# Read packet : PT_ADD_CELLPHONE_NO")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_ADD_CELLPHONE_NO_SUCC_U ptAddCellphoneNoSuccU;
	memcpy(ptAddCellphoneNoSuccU.CELLPHONE_NO, Data.CELLPHONE_NO, 32);

	connectedUser->WritePacket(PT_ADD_CELLPHONE_NO_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_ADD_CELLPHONE_NO_SUCC_U( WriteBuffer, ptAddCellphoneNoSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo->CELLPHONE_NO, ptAddCellphoneNoSuccU.CELLPHONE_NO, 32);
}

VOID CGameIocp::onPT_UPDATE_PASSWORD(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_UPDATE_PASSWORD_FAIL_U);
	READ_PACKET(PT_UPDATE_PASSWORD);
	CLog::WriteLog(_T("# Read packet : PT_UPDATE_PASSWORD")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_UPDATE_PASSWORD_SUCC_U ptUpdatePasswordSuccU;
	memcpy(ptUpdatePasswordSuccU.NEW_PASSWORD, Data.NEW_PASSWORD, 32);

	connectedUser->WritePacket(PT_UPDATE_PASSWORD_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_UPDATE_PASSWORD_SUCC_U( WriteBuffer, ptUpdatePasswordSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo->PASSWORD, ptUpdatePasswordSuccU.NEW_PASSWORD, 32);
}

VOID CGameIocp::onPT_UPDATE_NICKNAME(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_UPDATE_NICKNAME_FAIL_U);
	READ_PACKET(PT_UPDATE_NICKNAME);
	CLog::WriteLog(_T("# Read packet : PT_UPDATE_NICKNAME")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_UPDATE_NICKNAME_SUCC_U ptUpdateNicknameSuccU;
	memcpy(ptUpdateNicknameSuccU.NEW_NICKNAME, Data.NEW_NICKNAME, 32);

	connectedUser->WritePacket(PT_UPDATE_NICKNAME_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_UPDATE_NICKNAME_SUCC_U( WriteBuffer, ptUpdateNicknameSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo->NICKNAME, ptUpdateNicknameSuccU.NEW_NICKNAME, 32);
}

VOID CGameIocp::onPT_UPDATE_CELLPHONE_NO(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_UPDATE_CELLPHONE_NO_FAIL_U);
	READ_PACKET(PT_UPDATE_CELLPHONE_NO);
	CLog::WriteLog(_T("# Read packet : PT_UPDATE_CELLPHONE_NO")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_UPDATE_CELLPHONE_NO_SUCC_U ptUpdateCellphoneNoSuccU;
	memcpy(ptUpdateCellphoneNoSuccU.NEW_CELLPHONE_NO, Data.NEW_CELLPHONE_NO, 32);

	connectedUser->WritePacket(PT_UPDATE_CELLPHONE_NO_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_UPDATE_CELLPHONE_NO_SUCC_U( WriteBuffer, ptUpdateCellphoneNoSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	memcpy(userAccountInfo->CELLPHONE_NO, ptUpdateCellphoneNoSuccU.NEW_CELLPHONE_NO, 32);
}

VOID CGameIocp::onPT_UPDATE_RECORD(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_UPDATE_RECORD_FAIL_U);
	READ_PACKET(PT_UPDATE_RECORD);
	CLog::WriteLog(_T("# Read packet : PT_UPDATE_RECORD")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_UPDATE_RECORD_SUCC_U ptUpdateRecordSuccU;
	ptUpdateRecordSuccU.INTEGRAL_POINTS = Data.INTEGRAL_POINTS;
	ptUpdateRecordSuccU.LEVEL = Data.LEVEL;
	ptUpdateRecordSuccU.DIAMONDS = Data.DIAMONDS; 
	ptUpdateRecordSuccU.COINS = Data.COINS;
	ptUpdateRecordSuccU.WIN_TIMES = Data.WIN_TIMES;
	ptUpdateRecordSuccU.BATTLE_TIMES = Data.BATTLE_TIMES;
	ptUpdateRecordSuccU.WIN_RATE = Data.WIN_RATE;
	ptUpdateRecordSuccU.SHOT_DOWN_PLANES =  Data.SHOT_DOWN_PLANES;
	ptUpdateRecordSuccU.SHOT_DOWN_JUNIOR_PLANES = Data.SHOT_DOWN_JUNIOR_PLANES;
	ptUpdateRecordSuccU.SHOT_DOWN_MIDDLE_PLANES = Data.SHOT_DOWN_MIDDLE_PLANES;
	ptUpdateRecordSuccU.SHOT_DOWN_SENIOR_PLANES = Data.SHOT_DOWN_SENIOR_PLANES;

	connectedUser->WritePacket(PT_UPDATE_RECORD_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_UPDATE_RECORD_SUCC_U( WriteBuffer, ptUpdateRecordSuccU));

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	userAccountInfo->INTEGRAL_POINTS = ptUpdateRecordSuccU.INTEGRAL_POINTS;
	userAccountInfo->LEVEL = ptUpdateRecordSuccU.LEVEL;
	userAccountInfo->DIAMONDS = ptUpdateRecordSuccU.DIAMONDS; 
	userAccountInfo->COINS = ptUpdateRecordSuccU.COINS;
	userAccountInfo->WIN_TIMES = ptUpdateRecordSuccU.WIN_TIMES;
	userAccountInfo->BATTLE_TIMES = ptUpdateRecordSuccU.BATTLE_TIMES;
	userAccountInfo->WIN_RATE = ptUpdateRecordSuccU.WIN_RATE;
	userAccountInfo->SHOT_DOWN_PLANES =  ptUpdateRecordSuccU.SHOT_DOWN_PLANES;
	userAccountInfo->SHOT_DOWN_JUNIOR_PLANES = ptUpdateRecordSuccU.SHOT_DOWN_JUNIOR_PLANES;
	userAccountInfo->SHOT_DOWN_MIDDLE_PLANES = ptUpdateRecordSuccU.SHOT_DOWN_MIDDLE_PLANES;
	userAccountInfo->SHOT_DOWN_SENIOR_PLANES = ptUpdateRecordSuccU.SHOT_DOWN_SENIOR_PLANES;
}

