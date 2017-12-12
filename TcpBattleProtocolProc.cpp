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
#include "DoubleBattleProtocol.h"
#include "DOUBLE_BATTLE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_ReadPacket.h"
#include "DOUBLE_BATTLE_PT_WritePacket.h"

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"
#include "GameCtrlParams.h"
#include "Random.h"
#include "ErrCodes.h"

// 3.0 �汾��ӵ�ͷ�ļ�
#include "BattleFieldParams.h"

using namespace std;

// Battle ��������
VOID CGameIocp::onPT_BATTLE_SEARCH_ROOM(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_SEARCH_ROOM\n");
	READ_PACKET(PT_BATTLE_SEARCH_ROOM);

	// ����ʱ��սʱĬ���û��Ѿ���½ ���û�״̬����Ϊ US_CHANNEL_ENTERED
	// US_CHANNEL_ENTERED �û���½���״̬
	connectedUser->SetStatus(US_CHANNEL_ENTERED);

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BATTLE_SEARCH_ROOM_FAIL_U);
	CLog::WriteLog(_T("# Read packet : PT_BATTLE_SEARCH_ROOM"));

	ROOM_TYPE roomType = (ROOM_TYPE)Data.ROOM_TYPE;

	// Ϊ���Ѱ����Ϸ����
	CRoom* Room = mRoomManager.QuickJoinRoom(connectedUser, roomType);

	// δ�ҵ����� ֪ͨ��� Ѱ�ҷ���ʧ��
	if (Room == NULL) {
		connectedUser->WritePacket(
			PT_BATTLE_SEARCH_ROOM_FAIL_U,
			WriteBuffer,
			WRITE_PT_DOUBLE_BATTLE_SEARCH_ROOM_FAIL_U(WriteBuffer, 1));

		return;
	}

	// �����״̬����Ϊ ���뷿��
	connectedUser->SetStatus(US_ROOM_ENTERED);

	// ֪ͨ����ҵ�����
	S_PT_BATTLE_SEARCH_ROOM_SUCC_U  ptBattleSearchRoomSuccU;
	memset(&ptBattleSearchRoomSuccU, 0, sizeof(ptBattleSearchRoomSuccU));

	// ROOM_ID ��ǰ����ָ��ֵ
	// CURRENT_USER_COUNT ��ǰ������������� �������� + �������
	// ROOM_STATUS ��ǰ����״̬
	// ROOM_STATUS = RM_READY �ﵽ��ʼ��Ϸ������Ҫ�� 
	// ROOM_STATUS = RM_WAITING δ�ﵽ��ʼ��Ϸ������Ҫ�� 
	ptBattleSearchRoomSuccU.ROOM_ID = (DWORD_PTR)this;
	ptBattleSearchRoomSuccU.ROOM_TYPE = Room->GetType();
	ptBattleSearchRoomSuccU.ROOM_STATUS = Room->GetStatus(); 
	ptBattleSearchRoomSuccU.CURRENT_USER_COUNT = Room->GetCurrentUserCount();
	ptBattleSearchRoomSuccU.SIDE_IN_GAME = connectedUser->GetSideInGame();

	connectedUser->WritePacket(
		PT_BATTLE_SEARCH_ROOM_SUCC_U,
		WriteBuffer,
		WRITE_PT_BATTLE_SEARCH_ROOM_SUCC_U(WriteBuffer, ptBattleSearchRoomSuccU));

	// ���䴦�� �ȴ� ״̬ ��������
	if (Room->GetStatus() == ROOM_STATUS_WAITING) {
		printf("RM_WAITING");
		return;
	}
	// ���䴦�� ���� ״̬ ֪ͨ����ҿ�ʼ��Ϸ
	else if (Room->GetStatus() == ROOM_STATUS_READY) {
		Room->GameStart();
	}
}

// Battle ��Ҳ�����������
VOID CGameIocp::onPT_BATTLE_ARRANGE_WEAPON(CConnectedUser* connectedUser, BYTE* packet)
{
	printf("PT_BATTLE_ARRANGE_WEAPON\n");
	READ_PACKET(PT_BATTLE_ARRANGE_WEAPON);

	CBattleField* BattleField = connectedUser->GetEnteredRoom()->GetBattleField();

	// ��ȡ��������
	ENUM_WEAPON_TYPE WeaponType = (ENUM_WEAPON_TYPE)Data.WEAPON_TYPE;
	// ������������
	ENUM_TROOPS TroopsIn = (ENUM_TROOPS)Data.TROOPS_IN;
	// ��ȡ��������
	float PosX = Data.POS_X;
	float PosY = Data.POS_Y;

	// ����������ս�� ����ȡ���ص����
	Weapon Weapon = BattleField->InputTroops(WeaponType, TroopsIn, PosX, PosY);

	// �������Ͷ�������
	S_PT_BATTLE_ARRANGE_WEAPON_SUCC_M ptBattleArrangeWeaponSuccM;
	memset(&ptBattleArrangeWeaponSuccM, 0, sizeof(S_PT_BATTLE_ARRANGE_WEAPON_SUCC_M));

	// �������� �������� �������� δ���ı� ֱ��ת��
	ptBattleArrangeWeaponSuccM.WEAPON_TYPE = Weapon.GetProperty().WP_TYPE;
	ptBattleArrangeWeaponSuccM.TROOPS_IN = Weapon.GetTroopsIn();
	ptBattleArrangeWeaponSuccM.POS_X = Weapon.GetPosX();
	ptBattleArrangeWeaponSuccM.POS_Y = Weapon.GetPosY();
	// ���������Ҫ�Ӽ���ս���󷵻صĶ����ж�ȡ
	ptBattleArrangeWeaponSuccM.WEAPON_TAG = Weapon.GetWeaponTag();
	// �����ٶ���Ҫ�Ӽ���ս���󷵻صĶ����ж�ȡ
	ptBattleArrangeWeaponSuccM.SPEED = Weapon.GetProperty().SPEED;
	
	// �����������ݵĻ�����
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	// ��ȡ������ڷ���
	CRoom* room = connectedUser->GetEnteredRoom();
	// �򷿼���������ҹ㲥����������Ϣ
	room->WriteAll(PT_BATTLE_ARRANGE_WEAPON_SUCC_M, WriteBuffer, WRITE_PT_BATTLE_ARRANGE_WEAPON_SUCC_M(WriteBuffer, ptBattleArrangeWeaponSuccM));
}