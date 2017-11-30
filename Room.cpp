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
#include "USER_PT_Structure.h"
#include "USER_PT_ReadPacket.h"
#include "USER_PT_WritePacket.h"

#include "DoubleBattleProtocol.h"
#include "DOUBLE_BATTLE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_ReadPacket.h"
#include "DOUBLE_BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

#include "ErrorCode.h"
#include "GameCtrlParams.h"

extern S_DOUBLE_BATTLE_ROOM_USER GetDoubleBattleRoomUserPacket 
	(S_USER_ACCOUNT_INFO* userAccountInfo, S_USER_EQUIPMENT_INFO* userEquipmentInfo);

CRoom::CRoom(void)
{
	mType = RM_NO_TYPE;
	mStatus = RM_EMPTY;
	mIndex = 0;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;
}

CRoom::~CRoom(void)
{
}

// ����Ļ�ϴ�ӡ����Э��
void CRoom::printProtocol(int protocol)
{	
	switch (protocol)
	{
	case PT_DOUBLE_BATTLE_START_GAME_M:
		printf("PT_DOUBLE_BATTLE_START_GAME_M\n");
		break;
	}
}

// CRoom��Ŀ�ʼ����
// ͨ������ת����������
// �� RoomManager ����, ���� Begin( i ) ����ʱ, ͨ�� for ѭ������ i �����ɵ� Room �ı�Ŷ�Ϊ i
bool CRoom::Begin(DWORD index)
{
	CThreadSync Sync;

	mType = RM_NO_TYPE;
	mStatus = RM_EMPTY;
	mIndex = index;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;

	return true;
}

//��������ĺ���
bool CRoom::End(void)
{
	CThreadSync Sync;

	mType = RM_NO_TYPE;
	mStatus = RM_EMPTY;
	mIndex = 0;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;

	return true;
}

// �û����뷿��ʱ���д���ĺ���
bool CRoom::JoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType)
{
	CThreadSync Sync;

	// �����������Ѿ�ȷ�����뵱ǰ������Ϸ�ķ������Ͳ��� ��������
	if (mType != RM_NO_TYPE && mType != roomType)
		return false;

	// �����ָ����Ч ����
	if (!connectedUser)
		return false;

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};

	int capacityOfRoom, capacityOfTeam;

	// �����ݵ�ǰ�������� ���ú���������������
	if (mType == RM_TYPE_1V1) {
		capacityOfRoom = NUM_OF_PLAYERS_FOR_1V1;
		capacityOfTeam = NUM_OF_PLAYERS_FOR_1V1 / 2;
	}
	else if (mType == RM_TYPE_2V2) {
		capacityOfRoom = NUM_OF_PLAYERS_FOR_2V2;
		capacityOfTeam = NUM_OF_PLAYERS_FOR_2V2 / 2;
	}

	// ����������δ������ ���Ƚ���ҷ��䵽����
	if (mUsersInBlueTeam.size() < capacityOfTeam) {
		mUsersInBlueTeam.push_back(connectedUser);
		connectedUser->SetEnteredRoom(this);
	}
	// �����������Ѵ����� ����ҷ��䵽���
	else if (mUsersInRedTeam.size() < capacityOfTeam) {
		mUsersInRedTeam.push_back(connectedUser);
		connectedUser->SetEnteredRoom(this);
	}

	// ????????????????????????????????????????????????????
	// �˴���Ҫ�� CConnectedUser ��������ҵĶ��ѺͶ��� ��������
	

	// ����ǰ��Ҽ��뷿��� ����������������Ϸ��ʼ���� ���������Ϊ RM_READY
	if (mUsersInBlueTeam.size() + mUsersInRedTeam.size() == capacityOfRoom) {
		mStatus = RM_READY;
	}
	// ����ǰ��Ҽ��뷿��� ������������Ȼ��������Ϸ��ʼ���� ������״̬����Ϊ RM_WAITINIG
	else
	{
		mStatus = RM_WAITING;
	}

	// ��ǰ������������ҹ㲥����б������Ϣ


	//// ��ǰ������������ҹ㲥����б������Ϣ
	//list<CConnectedUser*>::iterator iter;
	//S_PT_DOUBLE_BATTLE_UPDATE_USER_LIST_M ptDoubleBattleUpdateUserListM;
	//memset(&ptDoubleBattleUpdateUserListM, 0, sizeof(ptDoubleBattleUpdateUserListM));
	//CHAR* Pointer = ptDoubleBattleUpdateUserListM.USER_LIST;

	//for(iter = mUserList.begin(); iter != mUserList.end(); ++iter) {
	//	CConnectedUser* ConnectedUser = (CConnectedUser*)(*iter);
	//	S_USER_ACCOUNT_INFO* userAccountInfo = ConnectedUser->GetUserAccountInfo();
	//	S_USER_EQUIPMENT_INFO* userEquipmentInfo = ConnectedUser->GetUserEquipmentInfo();

	//	S_DOUBLE_BATTLE_ROOM_USER DoubleBattleRoomUser = GetDoubleBattleRoomUserPacket(userAccountInfo, userEquipmentInfo);

	//	memcpy(Pointer, &DoubleBattleRoomUser, sizeof(DoubleBattleRoomUser));
	//	Pointer += sizeof(DoubleBattleRoomUser);
	//	ptDoubleBattleUpdateUserListM.LENGTH += sizeof(DoubleBattleRoomUser);
	//}

	//ptDoubleBattleUpdateUserListM.CURRENT_USER_COUNT = mUserList.size();

	//WriteAll(PT_DOUBLE_BATTLE_UPDATE_USER_LIST_M, WriteBuffer, WRITE_PT_DOUBLE_BATTLE_UPDATE_USER_LIST_M(WriteBuffer, ptDoubleBattleUpdateUserListM));

	return true;
}

void CRoom::SetUserStatusAll(USER_STATUS status)
{
	CThreadSync Sync;
	
	list<CConnectedUser*>::iterator iter;
	
	for(iter = mUsersInBlueTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->SetStatus(status);
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->SetStatus(status);
	}
}

 // �û��˳�����ʱ���õĺ���
 // ȷ���Ƿ������¼���Ƴ�����Ĳ���isDisconnected, һ�����˳�����ʱ������Ϸ�Ĳ���iocp
 // �Ƴ��û�����connectedUser
bool CRoom::LeaveRoom(bool isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser)
{
	CThreadSync Sync;

	//�����������ݰ�����ʱ������
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
	S_PT_DOUBLE_BATTLE_USER_LEAVE_M ptDoubleBattleUserLeaveM;

	// ���û��������������, ʧ��
	if (!connectedUser)
		return false;

	list<CConnectedUser*>::iterator iter;

	// �����״̬Ϊ���뷿�� ��δ��ʼ��Ϸ
	// �ں��������б��м�����ǰ��Ҳ�ɾ��
	// ֪ͨ������������� �����˳���ǰ����
	if(connectedUser->GetStatus() <= US_ROOM_ENTERED) {

		for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) 
		{
			CConnectedUser* ConnectedUser = (CConnectedUser*)(*iter);
			if(ConnectedUser == connectedUser) 
			{
				mUsersInBlueTeam.erase(iter);
				ptDoubleBattleUserLeaveM.USER_ID = ConnectedUser->GetUserAccountInfo()->USER_ID;
				WriteAll(PT_DOUBLE_BATTLE_USER_LEAVE_M, 
								WriteBuffer, 
								WRITE_PT_DOUBLE_BATTLE_USER_LEAVE_M(WriteBuffer, ptDoubleBattleUserLeaveM));
				return true;
			}
		}

		for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter)
		{
			CConnectedUser* ConnectedUser = (CConnectedUser*)(*iter);
			if (ConnectedUser == connectedUser)
			{
				mUsersInRedTeam.erase(iter);
				ptDoubleBattleUserLeaveM.USER_ID = ConnectedUser->GetUserAccountInfo()->USER_ID;
				WriteAll(PT_DOUBLE_BATTLE_USER_LEAVE_M,
					WriteBuffer,
					WRITE_PT_DOUBLE_BATTLE_USER_LEAVE_M(WriteBuffer, ptDoubleBattleUserLeaveM));
				return true;
			}
		}

		// ���û��б���û�м�������Ϸ��� ���� false
		return false;

	} 
	// ����ʱ���״̬Ϊ������Ϸ ��ʱ���ܼ��˳�����
	// Ӧ��AI�ӹ���Ϸ �� �ȴ���Ϸ�������˳�����
	else if(connectedUser->GetStatus() <= US_GAME_IN_PROGRESS){

		connectedUser->SetStatus(connectedUser->GetStatus() + 4); // ��Ϸ��ʼ���û��Ͽ�����, �� AI �ӹ�
		return true;
	}

	return false;
}

// �򷿼��ڵ������û��������ݰ�
bool CRoom::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (protocol <= 0 || !packet)
		return false;
	
	// ����Ļ�ϴ�ӡ�������ݰ���Э����Ϣ
	printProtocol(protocol);

	// ������ǰ���������û��б� Ⱥ������
	list<CConnectedUser*>::iterator iter;
	
	for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->WritePacket(protocol, packet, packetLength);

		// ÿ���1�����ݷ��ͺ� ����һ��ʱ�� ��ֹ����
		Sleep(10);
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		connectedUser->WritePacket(protocol, packet, packetLength);

		// ÿ���1�����ݷ��ͺ� ����һ��ʱ�� ��ֹ����
		Sleep(10);
	}

	return true;
}

// ������ʼʱ���õĺ���
bool CRoom::GameStart(void)
{
	CThreadSync Sync;

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};

	// ���÷������������״̬Ϊ US_GAME_START
	SetUserStatusAll(US_GAME_START);
	// ���÷���״̬Ϊ RM_GAME_START
	SetStatus(RM_GAME_START);
	// ���÷�����Ϸ��ʼ��־λΪ true
	mIsGameStarted = true;

	// ����ս��
	mBattleField = new CBattleField();
	mBattleField->Begin();

	// ��������б� ����ҽ�����Ϸ��ʼǰ�ĳ�ʼ��
	list<CConnectedUser*>::iterator iter;
	for(iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if(!connectedUser->InitializeForGameStart()) {
			return false;
		}
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if (!connectedUser->InitializeForGameStart()) {
			return false;
		}
	}

	// ֪ͨ�������Ϸ��ʼ ����Ϸ��ʼ��Ϣ�ɹ�����֮ǰ ���ܽ�����״̬ mStatus ����Ϊ RM_GAME_IN_PROGRESS
	S_PT_DOUBLE_BATTLE_START_GAME_M ptDoubleBattleStartGameM;
	memset(&ptDoubleBattleStartGameM, 0 , sizeof(ptDoubleBattleStartGameM));
	time(&ptDoubleBattleStartGameM.START_TIME);
	ptDoubleBattleStartGameM.ROOM_STATUS = GetStatus();
	WriteAll(PT_DOUBLE_BATTLE_START_GAME_M, WriteBuffer, WRITE_PT_DOUBLE_BATTLE_START_GAME_M(WriteBuffer, ptDoubleBattleStartGameM));

	// ��Ϸ��ʼ��Ϣ���ͳɹ��� ������״̬����Ϊ RM_GAME_IN_PROGRESS
	SetStatus(RM_GAME_IN_PROGRESS);

	return true;
}

// ��Ϸ����ʱ, �������HP�ܺ�, ���һ����ʤ
// ���������ｫ��Ϸ������, �û���Ϣ�ĸı������ݿ����ͬ��
// �򷿼��������û�������Ϸ������ݰ�
// �����趨�û�״̬, ��Ϊ�ȴ�, ��ʼ����Ϸ��ر���
bool CRoom::GameEnd(CGameIocp *iocp)
{
	CThreadSync Sync;

	mStatus = RM_GAME_ENDING;
	mIsGameStarted = false;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	S_PT_DOUBLE_BATTLE_END_M ptDoubleBattleEndM;
	memset(&ptDoubleBattleEndM, 0, sizeof(ptDoubleBattleEndM));

	//????????????????????????????????????
	// �˴�Ӧ�ö�ȡ��Ϸ��ս�����Ϣ �ַ���ÿ����Ϸ���

	// ��������б� ���������Ϸ����
	list<CConnectedUser*>::iterator iter;
	for (iter = mUsersInBlueTeam.begin(); iter != mUsersInBlueTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if (!connectedUser->GameEnd()) {
			return false;
		}
	}

	for (iter = mUsersInRedTeam.begin(); iter != mUsersInRedTeam.end(); ++iter) {
		CConnectedUser* connectedUser = (CConnectedUser*)(*iter);
		if (!connectedUser->GameEnd()) {
			return false;
		}
	}

	mType = RM_NO_TYPE;
	mStatus = RM_EMPTY;
	mUsersInBlueTeam.clear();
	mUsersInRedTeam.clear();
	mIsGameStarted = false;

	// ����ս��
	delete mBattleField;
	mBattleField = NULL;

	return true;
}

void CRoom::BroadcastBattleSituation(CGameIocp *iocp)
{
	CThreadSync Sync;

	// ��ȡʣ����Ϸʱ��
	int RemainGameTime = mBattleField->GetRemainGameTime();

	// ��ȡս���к���˫�������洢��̬������׵�ַ
	Weapon* HeaderOfTroopsInBlueTeam = mBattleField->getTroopInBlueTeam(0);
	Weapon* HeaderOfTroopsInRedTeam = mBattleField->getTroopInRedTeam(0);

	// ��ȡս���к���˫�������洢��̬����Ĵ�С
	int sizeOfTroopsInBlueTeam = mBattleField->getCountOfTroopsInBlueTeam();
	int sizeOfTroopsInRedTeam = mBattleField->getCountOfTroopsInRedTeam();

	// �����㲥���ݻ�����
	BYTE PacketBuffer[MAX_BUFFER_LENGTH] = { 0, };
	// ���ù㲥Э���
	int Protocol = 111; // �˴�Э�����Ҫ���¶���?????????????????????????
	// ����㲥���ݰ�����
	int PacketLength = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(Weapon) * sizeOfTroopsInBlueTeam + sizeof(Weapon) * sizeOfTroopsInRedTeam;

	// ��ʣ����Ϸʱ��д�뻺����
	memcpy(PacketBuffer, &RemainGameTime, sizeof(int));
	// ������������̬�洢�����Сд�뻺����
	memcpy(PacketBuffer + sizeof(int), &sizeOfTroopsInBlueTeam, sizeof(int));
	// ���췽������̬�洢�����Сд�뻺����
	memcpy(PacketBuffer + sizeof(int) + sizeof(int), &sizeOfTroopsInRedTeam, sizeof(int));
	// ������������̬�洢����д�뻺����
	memcpy(PacketBuffer + sizeof(int) + sizeof(int) + sizeof(int), HeaderOfTroopsInBlueTeam, sizeof(Weapon) * sizeOfTroopsInBlueTeam);
	// ���췽������̬�洢����д�뻺����
	memcpy(PacketBuffer + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(Weapon) * sizeOfTroopsInBlueTeam, HeaderOfTroopsInRedTeam, sizeof(Weapon) * sizeOfTroopsInRedTeam);

	// �򷿼��������û����͹㲥����
	WriteAll(Protocol, PacketBuffer, PacketLength);
}