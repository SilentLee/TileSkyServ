#include "stdafx.h"

#include "Room.h"

//#include "CriticalSection.h"
//#include "MultiThreadSync.h"
//#include "CircularQueue.h"
//#include "NetworkSession.h"
//#include "PacketSession.h"
//#include "Iocp.h"
//#include "Stream.h"
#include "MemoryPool.h"

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_WritePacket.h"

//#include "ErrorCode.h"


//RM_EMPTY = 0,
//RM_WAITING,
//RM_READY,
//RM_GAME_START,
//RM_GAME_IN_PROGRESS,
//RM_GAME_ENDING,
//RM_GAME_ENDED

ROOM_STATUS CRoom::UpdateGameProcess()
{
	CThreadSync Sync;
	 
	if (mBattleField != nullptr) {

		// ���¶�ս���� ��ȡ��ʱս��״̬
		ENUM_BATTLE_STATUS BattleStatus = mBattleField->UpdateBattleFieldSituation();

		// ���·���״̬
		// ��ս������ ����״̬����Ϊ��Ϸ������
		if (BattleStatus == BATTLE_STATUS_IN_PROGRESSING) {
			mStatus = ROOM_STATUS_GAME_IN_PROGRESS;
			// �򷿼�����ҹ㲥��ǰս��̬��
			//BroadcastBattleSituation();
		}
		// ����ȡʤ ����״̬����Ϊ��Ϸ������
		else if (BattleStatus == BATTLE_STATUS_BLUE_WIN) {
			mStatus = ROOM_STATUS_GAME_ENDING;
		}
		// �췽ȡʤ ����״̬����Ϊ��Ϸ������
		else if (BattleStatus == BATTLE_STATUS_RED_WIN) {
			mStatus = ROOM_STATUS_GAME_ENDING;
		}
		// ƽ�� ����״̬����Ϊ��Ϸ������
		else if (BattleStatus == BATTLE_STATUS_DRAW) {
			mStatus = ROOM_STATUS_GAME_ENDING;
		}
	}

	// ���ص�ǰ����״̬
	return mStatus;
}

// ��ͻ���ͬ����Ϸ���̵ĺ���
ROOM_STATUS CRoom::SyncGameProcess()
{
	CThreadSync Sync;

	if (mBattleField != nullptr) {

		ENUM_BATTLE_STATUS BattleStatus = mBattleField->GetStatus();

		// ��ս������ ����״̬����Ϊ��Ϸ������
		if (BattleStatus == BATTLE_STATUS_IN_PROGRESSING) {
			mStatus = ROOM_STATUS_GAME_IN_PROGRESS;
			// �򷿼�����ҹ㲥��ǰս��̬��
			BroadcastBattleSituation();
		}
		// ����ȡʤ ����״̬����Ϊ��Ϸ������
		else if (BattleStatus == BATTLE_STATUS_BLUE_WIN) {
			mStatus = ROOM_STATUS_GAME_ENDING;
		}
		// �췽ȡʤ ����״̬����Ϊ��Ϸ������
		else if (BattleStatus == BATTLE_STATUS_RED_WIN) {
			mStatus = ROOM_STATUS_GAME_ENDING;
		}
		// ƽ�� ����״̬����Ϊ��Ϸ������
		else if (BattleStatus == BATTLE_STATUS_DRAW) {
			mStatus = ROOM_STATUS_GAME_ENDING;
		}
	}

	// ���ص�ǰ����״̬
	return mStatus;
}

void CRoom::BroadcastBattleSituation()
{
	CThreadSync Sync;

	// �������ս���е����б���
	// ��ȡ����������̬���鳤��
	int SizeOfTroopsInBlueTeam = mBattleField->getCountOfTroopsInBlueTeam();
	// �������ݻ�����
	BYTE PacketBufferTroopsInBlueTeam[3000] = {0, };
	// ���������������ݰ�����
	int LengthOfPacketBufferTroopsInBlueTeam = 0;
	// ����ǰ�洢���������Ķ�̬���鳤�ȴ��� 0
	if (SizeOfTroopsInBlueTeam > 0) {
		// ��ȡ����������̬�����׵�ַ
		Weapon* HeaderOfTroopsInBlueTeam = mBattleField->getTroopInBlueTeam(0);
		// д������
		memcpy(PacketBufferTroopsInBlueTeam, HeaderOfTroopsInBlueTeam, sizeof(Weapon)* SizeOfTroopsInBlueTeam);
		// ���������������ݰ�����
		LengthOfPacketBufferTroopsInBlueTeam = sizeof(Weapon)* SizeOfTroopsInBlueTeam;
	}
	// ����ǰ�洢���������Ķ�̬���鳤�ȵ��� 0
	// ���ܷ��ʸ����� ���������Խ��
	// ����ֻ�洢���鳤�� �� 0
	else {
		// ���������������ݰ�����
		LengthOfPacketBufferTroopsInBlueTeam = 0;
	}

	// ����췽ս���е����б���
	// ��ȡ�췽������̬���鳤��
	int SizeOfTroopsInRedTeam = mBattleField->getCountOfTroopsInRedTeam();
	// �������ݻ�����
	BYTE PacketBufferTroopsInRedTeam[3000] = {0, };
	// �����췽�������ݰ�����
	int LengthOfPacketBufferTroopsInRedTeam = 0;
	// ����ǰ�洢�췽�����Ķ�̬���鳤�ȴ��� 0
	if (SizeOfTroopsInRedTeam > 0) {
		// ��ȡ�췽������̬�����׵�ַ
		Weapon* HeaderOfTroopsInRedTeam = mBattleField->getTroopInRedTeam(0);
		// д������
		memcpy(PacketBufferTroopsInRedTeam, HeaderOfTroopsInRedTeam, sizeof(Weapon)* SizeOfTroopsInRedTeam);
		// ����췽�������ݰ�����
		LengthOfPacketBufferTroopsInRedTeam = sizeof(Weapon)* SizeOfTroopsInRedTeam;
	}
	// ����ǰ�洢���������Ķ�̬���鳤�ȵ��� 0
	// ���ܷ��ʸ����� ���������Խ��
	// ����ֻ�洢���鳤�� �� 0
	else {
		// ����췽�������ݰ�����
		LengthOfPacketBufferTroopsInRedTeam = 0;
	}

	S_PT_BATTLE_UPDATE_SITUATION_M ptBattleUpdateSituationM;
	memset(&ptBattleUpdateSituationM, 0, sizeof(S_PT_BATTLE_UPDATE_SITUATION_M));
	// д�뵱ǰ��Ϸʣ��ʱ��
	ptBattleUpdateSituationM.REMAINING_GAME_TIME = mBattleField->GetRemainGameTime();
	// д�������������ݳ���
	ptBattleUpdateSituationM.BLUE_TROOPS_DATA_LENGTH = SizeOfTroopsInBlueTeam * sizeof(Weapon);
	// д��췽�������ݳ���
	ptBattleUpdateSituationM.RED_TROOPS_DATA_LENGTH = SizeOfTroopsInRedTeam * sizeof(Weapon);
	// д������������Ϊ���ݳ���
	ptBattleUpdateSituationM.BLUE_TROOPS_ACTION_DATA_LENGTH = 0;
	// д��췽������Ϊ���ݳ���
	ptBattleUpdateSituationM.RED_TROOPS_ACTION_DATA_LENGTH = 0;
	// д��������������
	memcpy(ptBattleUpdateSituationM.DATA, PacketBufferTroopsInBlueTeam, LengthOfPacketBufferTroopsInBlueTeam);
	// д��췽��������
	memcpy(ptBattleUpdateSituationM.DATA + LengthOfPacketBufferTroopsInBlueTeam, PacketBufferTroopsInRedTeam, LengthOfPacketBufferTroopsInRedTeam);
	// �˴�Ӧд������������Ϊ����
	//
	// �˴�Ӧд��췽������Ϊ����
	//

	// �򷿼�����ҹ㲥����
	WriteAll(PT_BATTLE_UPDATE_SITUATION_M, (BYTE*)&ptBattleUpdateSituationM, sizeof(S_PT_BATTLE_UPDATE_SITUATION_M));
}