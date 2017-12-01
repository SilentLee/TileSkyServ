#pragma once
#include "MySqlSessionPool.h"
#include "MySqlSession.h"
#include "MySqlResReader.h"
#include "USER_PT_Structure.h"
#include "PURCHASE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_Structure.h"

// ���û�HP AP �仯��ʱ��, ���û����ͱ仯���ݰ���Macro����
// ����û���ǰ״̬
// ����:
// ERROR_STATUS ���滻Ϊ != US_NONE
// PROTOCOL ���滻Ϊ PT_CHANNEL_ENTER_FAIL_U
// 
#define CHECK_USER_STATUS(ERROR_STATUS, PROTOCOL)\
	if (connectedUser->GetStatus() ERROR_STATUS)\
	{\
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	EC_USER_STATUS_INCORRECT));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, EC_USER_STATUS_INCORRECT);\
	return;\
	}

// �����з�����ز�����ʱ��, ���ʧ�ܵĻ�, ����ʧ�����ݰ���Macro����
#define ROOM_OBJCT_FAIL(PROTOCOL)\
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	EC_ROOM_OBJECT_POINTER_FAILED));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, EC_ROOM_OBJECT_POINTER_FAILED);

// ���в���ʧ�ܵ������, ����ʧ�����ݰ���Macro����
#define WRITE_FAIL_U_PACKET(PROTOCOL, ERROR_CODE) \
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	ERROR_CODE));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, ERROR_CODE);

// ����������ʽʧ�����ݰ���Macro����
#define WRITE_FAIL_U_PACKET2(OBJECT, PROTOCOL, ERROR_CODE) \
	OBJECT->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	ERROR_CODE));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, ERROR_CODE);

// ��ȡ����õ����ݰ���Macro����
#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(packet, Data);

static INT		Time		= 0;
static INT		Sequence	= 0;

// ������Ŀ��ŵĺ���
static INT64	MAKE_SERIAL(INT itemType, INT equipLocation, INT sellType, INT createType)
{
	time_t	Raw;
	time(&Raw);

	INT		CurrentTime = (INT) Raw;
	INT64	Serial		= 0;

	// ����ʱ��ֵ����������ֵ
	Sequence	= (CurrentTime == Time ? Sequence + 1 : 0);
	// 32bit ����ʱ��ֵ
	Serial		= CurrentTime | (Serial << 32); // 16bit����
	Serial		= itemType | (Serial << 16);
	Serial		= equipLocation | (Serial << 4);
	Serial		= sellType | (Serial << 4);
	Serial		= createType | (Serial << 4);
	Serial		= Sequence | (Serial << 4);

	Time		= CurrentTime;

	return Serial;
}

// �е�����������ͨ��, �̳�CIocp��
class CGameIocp : public CIocp
{
public:
	CGameIocp(VOID);
	virtual ~CGameIocp(VOID);

private:
	CNetworkSession			mListenSession; // �е�Listen�Ķ���
	CConnectedUserManager	mConnectedUserManager; // �����û���CConnectedUserManager
	CRoomManager			mRoomManager; // �������CRoomManager

	// �����쳣��ֹ�Ŀͻ��˲����д����KeepAlive�߳�ģ��
	HANDLE					mKeepThreadHandle;
	HANDLE					mKeepThreadDestroyEvent;

	// �������ʱ�估������Ϸ���ʱ�����Ϸ�߳�ģ��
	HANDLE					mGameThreadHandle;
	HANDLE					mGameThreadDestroyEvent;

	CMySqlSessionPool mMySqlSessionPool;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	VOID	KeepThreadCallback(VOID);
	VOID	GameThreadCallback(VOID);

protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);

private:
	// 1v1 �����е�Э�鴦����
	VOID onPT_BATTLE_1V1_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet);
	VOID onPT_BATTLE_1V1_ARRANGE_CARD(CConnectedUser* connectedUser, BYTE *packet);

private:
	// Э�鴦����
	// �û��˻�ע�� ��¼ ��Ϣ�������ݰ�����
	VOID onPT_VISITOR_REGIST(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_USER_NAME_REGIST(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_BIND_ACCOUNT_REGIST(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_CELLPHONE_NO_REGIST(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_USER_NAME_LOGIN(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_BIND_ACCOUNT_LOGIN(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_CELLPHONE_NO_LOGIN(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_VISITOR_DEFAULT_LOGIN(CConnectedUser* connectedUser, BYTE *packet);
	
	VOID onPT_ADD_NICKNAME(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_ADD_BIND_ACCOUNT(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_ADD_CELLPHONE_NO(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_UPDATE_PASSWORD(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_UPDATE_NICKNAME(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_UPDATE_CELLPHONE_NO(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_UPDATE_RECORD(CConnectedUser* connectedUser, BYTE *packet);

	// �û������������ݰ�����
	VOID onPT_BUY_COINS(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_BUY_DIAMONDS(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_BUY_CARDS(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_ACQUIRE_CHEST(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_OPEN_SUPPLY_CHEST(CConnectedUser* connectedUser, BYTE *packet);
	VOID onPT_UPGRADE_CARD(CConnectedUser* connectedUser, BYTE *packet);

	// �ͻ���װ�������������ݰ�����
	VOID onPT_LOAD_CARDS_ON_SALE(CConnectedUser* connectedUser, BYTE* packet);

	// �������� ��Ϸ���ݰ�����Э��
	VOID onPT_DOUBLE_BATTLE_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet);

	// ���յ��ɻ�������Ϣ
	VOID onPT_DOUBLE_BATTLE_INSERT_JUNIOR_PLANE(CConnectedUser* connectedUser, BYTE* packet);
	VOID onPT_DOUBLE_BATTLE_INSERT_MIDDLE_PLANE(CConnectedUser* connectedUser, BYTE* packet);
	VOID onPT_DOUBLE_BATTLE_INSERT_SENIOR_PLANE(CConnectedUser* connectedUser, BYTE* packet);
	VOID onPT_DOUBLE_BATTLE_INSERT_JUNIOR_STEALTH_PLANE(CConnectedUser* connectedUser, BYTE* packet);
	VOID onPT_DOUBLE_BATTLE_INSERT_MIDDLE_STEALTH_PLANE(CConnectedUser* connectedUser, BYTE* packet);
	VOID onPT_DOUBLE_BATTLE_INSERT_SENIOR_STEALTH_PLANE(CConnectedUser* connectedUser, BYTE* packet);

	// ���յ�����������Ϣ
	VOID onPT_DOUBLE_BATTLE_INSERT_SENIOR_ANTI_STEALTH_RADAR(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_MIDDLE_ANTI_STEALTH_RADAR(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_JUNIOR_ANTI_STEALTH_RADAR(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_SENIOR_MISSILE(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_MIDDLE_MISSILE(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_JUNIOR_MISSILE(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_SENIOR_RADAR(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_MIDDLE_RADAR(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_JUNIOR_RADAR(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_SENIOR_CANNONBALL(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_MIDDLE_CANNONBALL(CConnectedUser* connectedUser, BYTE* packet);
    VOID onPT_DOUBLE_BATTLE_INSERT_JUNIOR_CANNONBALL(CConnectedUser* connectedUser, BYTE* packet);

	// ���ݿ��д����
	// �û��˻�ע�� ��¼ ��Ϣ�������ݰ�����
	INT db_VisitorRegist();
	INT db_UserNameRegist(S_PT_USER_NAME_REGIST ptUserNameRegist);
	INT db_BindAccountRegist(S_PT_BIND_ACCOUNT_REGIST ptBindAccountRegist);
	INT db_CellphoneNoRegist(S_PT_CELLPHONE_NO_REGIST ptCellphoneNoRegist);
	S_PT_USER_NAME_LOGIN_SUCC_U* db_UserNameLogin(S_PT_USER_NAME_LOGIN ptUserNameLogin);
	S_PT_BIND_ACCOUNT_LOGIN_SUCC_U* db_BindAccountLogin(S_PT_BIND_ACCOUNT_LOGIN ptBindAccountLogin);
	S_PT_CELLPHONE_NO_LOGIN_SUCC_U* db_CellphoneNoLogin(S_PT_CELLPHONE_NO_LOGIN ptCellphoneNoLogin);
	S_PT_VISITOR_DEFAULT_LOGIN_SUCC_U* db_VisitorDefaultLogin(S_PT_VISITOR_DEFAULT_LOGIN ptVisitorDefaultLogin);
	S_PT_USER_NAME_DEFAULT_LOGIN_SUCC_U* db_UserNameDefaultLogin(S_PT_USER_NAME_DEFAULT_LOGIN ptUserNameDefaultLogin);
	S_PT_BIND_ACCOUNT_DEFAULT_LOGIN_SUCC_U* db_BindAccountDefaultLogin(S_PT_BIND_ACCOUNT_DEFAULT_LOGIN ptBindAccountDefaultLogin);
	S_PT_CELLPHONE_NO_DEFALUT_LOGIN_SUCC_U* db_CellphoneNoDefaultLogin(S_PT_CELLPHONE_NO_DEFAULT_LOGIN ptCellphonNoDefaultLogin);

	S_PT_ADD_NICKNAME_SUCC_U* db_AddNickname(S_PT_ADD_NICKNAME ptAddNickname);
	S_PT_ADD_BIND_ACCOUNT_SUCC_U* db_AddBindAccount(S_PT_ADD_BIND_ACCOUNT ptAddBindAccount);
	S_PT_ADD_CELLPHONE_NO_SUCC_U* db_AddCellphoneNo(S_PT_ADD_CELLPHONE_NO ptAddCellphoneNo);

	S_PT_UPDATE_PASSWORD_SUCC_U* db_UpdatePassword(S_PT_UPDATE_PASSWORD ptUpdatePassword);
	S_PT_UPDATE_NICKNAME_SUCC_U* db_UpdateNickname(S_PT_UPDATE_NICKNAME ptUpdateNickname);
	S_PT_UPDATE_CELLPHONE_NO_SUCC_U* db_UpdateCellphoneNo(S_PT_UPDATE_CELLPHONE_NO ptUpdateCellphoneNo);
	S_PT_UPDATE_RECORD_SUCC_U* db_UpdateRecord(S_PT_UPDATE_RECORD ptUpdateRecord);

	// �û������������ݰ�����
	S_PT_BUY_COINS_SUCC_U* db_BuyCoins(S_PT_BUY_COINS ptBuyCoins);
	S_PT_BUY_DIAMONDS_SUCC_U* db_BuyDiamonds(S_PT_BUY_DIAMONDS ptBuyDiamonds);
	S_PT_BUY_CARDS_SUCC_U* db_BuyCards(S_PT_BUY_CARDS ptBuyCards);
	S_PT_OPEN_SUPPLY_CHEST_SUCC_U* db_GetSupplyChest(S_PT_OPEN_SUPPLY_CHEST ptGetSupplyChest);

	//// ��������
	int assignCards(int* cardType, int* numOfCards, int* levelOfCards, /*char* nameOfCards, */int kindsOfCards, int rangeOfCardNum, CConnectedUser* connectedUser);
	void loadCardsInSupplyChest(S_PT_OPEN_SUPPLY_CHEST_SUCC_U* ptGetSupplyChestSuccU, int* cardType, int* numOfCards, int* level, /*char* nameOfCards,*/ int kindsOfCard);
	BOOL updateDBwithDataInSupplyChest(S_PT_OPEN_SUPPLY_CHEST_SUCC_U* ptGetSupplyChestSuccU, int userId, char* password, int kindsOfCards);
};
