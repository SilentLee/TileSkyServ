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
#include "PurchaseProtocol.h"
#include "PURCHASE_PT_Structure.h"
#include "PURCHASE_PT_ReadPacket.h"
#include "PURCHASE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"
#include "GameCtrlParams.h"
#include "Random.h"

VOID CGameIocp::onPT_UPGRADE_CARD(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_LOAD_CARDS_ON_SALE_FAIL_U);
	READ_PACKET(PT_UPGRADE_CARD);
	CLog::WriteLog(_T("# Read packet : PT_UPGRADE_CARD")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	S_USER_EQUIPMENT_INFO* userEquipmentInfo = connectedUser->GetUserEquipmentInfo();

	S_PT_UPGRADE_CARD_SUCC_U ptUpgradeCardSuccU;
	memset(&ptUpgradeCardSuccU, 0 , sizeof(S_PT_UPGRADE_CARD_SUCC_U));

	switch(Data.CARD_LEVEL)
	{
	case 1:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_2) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_2;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_2;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_2;
		break;

	case 2:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_3) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_3;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_3;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_3;
		break;

	case 3:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_4) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_4;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_4;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_4;
		break;

	case 4:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_5) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_5;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_5;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_5;
		break;

	case 5:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_6) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_6;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_6;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_6;
		break;

	case 6:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_7) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_7;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_7;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_7;
		break;

	case 7:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_8) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_8;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_8;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_8;
		break;

	case 8:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_9) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_9;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_9;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_9;
		break;

	case 9:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_10) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_10;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_10;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_10;
		break;

	case 10:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_11) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_11;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_11;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_11;
		break;

	case 11:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_12) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_12;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_12;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_12;
		break;

	case 12:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_13) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_13;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_13;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_13;
		break;

	case 13:
		if(userAccountInfo->COINS < COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_14) {
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userAccountInfo->COINS -= COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_14;
		ptUpgradeCardSuccU.CARD_UPGRADE_COST = COST_OF_COINS_FOR_UPGRADE_TO_LEVEL_14;
		ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED = NUM_OF_CARDS_FOR_UPGRADE_TO_LEVEL_14;
		break;
	}

	switch(Data.CARD_TYPE)
	{
	case JUNIOR_PLANE:
		if(userEquipmentInfo->JUNIOR_PLANE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->JUNIOR_PLANE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->JUNIOR_PLANE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = JUNIOR_PLANE;
		break;

	case MIDDLE_PLANE:
		if(userEquipmentInfo->MIDDLE_PLANE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->MIDDLE_PLANE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->MIDDLE_PLANE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = MIDDLE_PLANE;
		break;

	case SENIOR_PLANE:
		if(userEquipmentInfo->SENIOR_PLANE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->SENIOR_PLANE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->SENIOR_PLANE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = SENIOR_PLANE;
		break;

	case JUNIOR_STEALTH_PLANE:
		if(userEquipmentInfo->JUNIOR_STEALTH_PLANE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->JUNIOR_STEALTH_PLANE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->JUNIOR_STEALTH_PLANE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = JUNIOR_STEALTH_PLANE;
		break;

	case MIDDLE_STEALTH_PLANE:
		if(userEquipmentInfo->MIDDLE_STEALTH_PLANE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->MIDDLE_STEALTH_PLANE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->MIDDLE_STEALTH_PLANE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = MIDDLE_STEALTH_PLANE;
		break;

	case SENIOR_STEALTH_PLANE:
		if(userEquipmentInfo->SENIOR_STEALTH_PLANE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->SENIOR_STEALTH_PLANE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->SENIOR_STEALTH_PLANE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = SENIOR_STEALTH_PLANE;
		break;

	case SENIOR_ANTI_STEALTH_RADAR:
		if(userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = SENIOR_ANTI_STEALTH_RADAR;
		break;

	case MIDDLE_ANTI_STEALTH_RADAR:
		if(userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = MIDDLE_ANTI_STEALTH_RADAR;
		break;

	case JUNIOR_ANTI_STEALTH_RADAR:
		if(userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = JUNIOR_ANTI_STEALTH_RADAR;
		break;

	case SENIOR_MISSILE:
		if(userEquipmentInfo->SENIOR_MISSILE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->SENIOR_MISSILE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->SENIOR_MISSILE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = SENIOR_MISSILE;
		break;

	case MIDDLE_MISSILE:
		if(userEquipmentInfo->MIDDLE_MISSILE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->MIDDLE_MISSILE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->MIDDLE_MISSILE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = MIDDLE_MISSILE;
		break;

	case JUNIOR_MISSILE:
		if(userEquipmentInfo->JUNIOR_MISSILE_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->JUNIOR_MISSILE_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->JUNIOR_MISSILE_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = JUNIOR_MISSILE;
		break;

	case SENIOR_RADAR:
		if(userEquipmentInfo->SENIOR_RADAR_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->SENIOR_RADAR_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->SENIOR_RADAR_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = SENIOR_RADAR;
		break;

	case MIDDLE_RADAR:
		if(userEquipmentInfo->MIDDLE_RADAR_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->MIDDLE_RADAR_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->MIDDLE_RADAR_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = MIDDLE_RADAR;
		break;

	case JUNIOR_RADAR:
		if(userEquipmentInfo->JUNIOR_RADAR_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->JUNIOR_RADAR_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->JUNIOR_RADAR_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = JUNIOR_RADAR;
		break;

	case SENIOR_CANNONBALL:
		if(userEquipmentInfo->SENIOR_CANNONBALL_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->SENIOR_CANNONBALL_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->SENIOR_CANNONBALL_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = SENIOR_CANNONBALL;
		break;

	case MIDDLE_CANNONBALL:
		if(userEquipmentInfo->MIDDLE_CANNONBALL_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->MIDDLE_CANNONBALL_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->MIDDLE_CANNONBALL_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = MIDDLE_CANNONBALL;
		break;

	case JUNIOR_CANNONBALL:
		if(userEquipmentInfo->JUNIOR_CANNONBALL_CARD_NUM < ptUpgradeCardSuccU.CARD_NUM_UPGRADE_NEED) {
			userAccountInfo->COINS += ptUpgradeCardSuccU.CARD_UPGRADE_COST;
			connectedUser->WritePacket(PT_UPGRADE_CARD_FAIL_U, WriteBuffer, WRITE_PT_UPGRADE_CARD_FAIL_U(WriteBuffer, 1));
			break;
		}
		userEquipmentInfo->JUNIOR_CANNONBALL_LEVEL += 1;
		ptUpgradeCardSuccU.CARD_LEVEL = userEquipmentInfo->JUNIOR_CANNONBALL_LEVEL;
		ptUpgradeCardSuccU.CARD_TYPE = JUNIOR_CANNONBALL;
		break;
	}

	connectedUser->WritePacket(PT_UPGRADE_CARD_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_UPGRADE_CARD_SUCC_U( WriteBuffer, ptUpgradeCardSuccU));
}

VOID CGameIocp::onPT_LOAD_CARDS_ON_SALE(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_LOAD_CARDS_ON_SALE_FAIL_U);
	READ_PACKET(PT_LOAD_CARDS_ON_SALE);
	CLog::WriteLog(_T("# Read packet : PT_LOAD_CARDS_ON_SALE")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体

	S_PT_LOAD_CARDS_ON_SALE_SUCC_U ptLoadCardsOnSaleSuccU;
	memset(&ptLoadCardsOnSaleSuccU, 0, sizeof(S_PT_LOAD_CARDS_ON_SALE_SUCC_U));
	ptLoadCardsOnSaleSuccU.KINDS_OF_CARDS_ON_SALE = KINDS_OF_CARDS_TO_SELL;

	switch(Data.USER_LEVEL)
	{
	case 1:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 2:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 3:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 4:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 5:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 6:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 7:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 8:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 9:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 10:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 11:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 12:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;

	case 13:
		ptLoadCardsOnSaleSuccU.CARD_1 = JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_1_COST = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptLoadCardsOnSaleSuccU.CARD_2 = SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_2_COST = COINS_COST_OF_SENIOR_MISSILE;
		ptLoadCardsOnSaleSuccU.CARD_3 = MIDDLE_STEALTH_PLANE;
		ptLoadCardsOnSaleSuccU.CARD_3_COST = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;
	}

	connectedUser->WritePacket(PT_LOAD_CARDS_ON_SALE_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_LOAD_CARDS_ON_SALE_SUCC_U( WriteBuffer, ptLoadCardsOnSaleSuccU));
}

VOID CGameIocp::onPT_BUY_COINS(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BUY_COINS_FAIL_U);
	READ_PACKET(PT_BUY_COINS);
	CLog::WriteLog(_T("# Read packet : PT_BUY_COINS")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体
	
	//S_PT_BUY_COINS_SUCC_U* ptBuyCoinsSuccU = db_BuyCoins(Data); // 此处是否存在内存泄漏?????
	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	S_PT_BUY_COINS_SUCC_U ptBuyCoinsSuccU;

	switch(Data.PACKET_LEVEL)
	{
	case PACKET_LEVEL_1:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_1) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_1;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_1;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_1;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_1;
		break;

	case PACKET_LEVEL_2:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_2) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_2;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_2;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_2;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_2;
		break;

	case PACKET_LEVEL_3:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_3) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_3;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_3;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_3;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_3;
		break;

	case PACKET_LEVEL_4:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_4) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_4;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_4;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_4;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_4;
		break;

	case PACKET_LEVEL_5:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_5) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_5;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_5;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_5;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_5;
		break;

	case PACKET_LEVEL_6:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_6) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_6;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_6;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_6;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_6;
		break;

	case PACKET_LEVEL_7:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_7) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_7;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_7;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_7;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_7;
		break;

	case PACKET_LEVEL_8:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_9) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_8;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_8;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_8;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_8;
		break;

	case PACKET_LEVEL_9:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_OF_COINS_PACKET_LEVEL_9) {
			connectedUser->WritePacket(PT_BUY_COINS_FAIL_U, WriteBuffer, WRITE_PT_BUY_COINS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS += COINS_PACKET_LEVEL_9;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_OF_COINS_PACKET_LEVEL_9;
		ptBuyCoinsSuccU.COINS = COINS_PACKET_LEVEL_9;
		ptBuyCoinsSuccU.DIAMONDS = DIAMONDS_COST_OF_COINS_PACKET_LEVEL_9;
		break;
	}
	ptBuyCoinsSuccU.PACKET_LEVEL = Data.PACKET_LEVEL;

	connectedUser->WritePacket(PT_BUY_COINS_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_BUY_COINS_SUCC_U(
		WriteBuffer,
		ptBuyCoinsSuccU));
}

VOID CGameIocp::onPT_BUY_DIAMONDS(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BUY_DIAMONDS_FAIL_U);
	READ_PACKET(PT_BUY_DIAMONDS);
	CLog::WriteLog(_T("# Read packet : PT_BUY_DIAMONDS")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体
	
	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	S_PT_BUY_DIAMONDS_SUCC_U ptBuyDiamondsSuccU;

	switch(Data.PACKET_LEVEL)
	{
	case PACKET_LEVEL_1:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_1;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_1;
		break;
	case PACKET_LEVEL_2:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_2;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_2;
		break;
	case PACKET_LEVEL_3:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_3;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_3;
		break;
	case PACKET_LEVEL_4:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_4;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_4;
		break;
	case PACKET_LEVEL_5:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_5;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_5;
		break;
	case PACKET_LEVEL_6:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_6;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_6;
		break;
	case PACKET_LEVEL_7:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_7;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_7;
		break;
	case PACKET_LEVEL_8:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_8;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_8;
		break;
	case PACKET_LEVEL_9:
		userAccountInfo->DIAMONDS += DIAMONDS_PACKET_LEVEL_9;
		ptBuyDiamondsSuccU.DIAMONDS = DIAMONDS_PACKET_LEVEL_9;
		break;
	}

	ptBuyDiamondsSuccU.PACKET_LEVEL = Data.PACKET_LEVEL;

	connectedUser->WritePacket(PT_BUY_DIAMONDS_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_BUY_DIAMONDS_SUCC_U(
		WriteBuffer,
		ptBuyDiamondsSuccU));
}

VOID CGameIocp::onPT_BUY_CARDS(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_BUY_CARDS_FAIL_U);
	READ_PACKET(PT_BUY_CARDS);
	CLog::WriteLog(_T("# Read packet : PT_BUY_CARDS")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体
	
	//S_PT_BUY_CARDS_SUCC_U* ptBuyCardsSuccU = db_BuyCards(Data);
	S_PT_BUY_CARDS_SUCC_U ptBuyCardsSuccU;
	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	S_USER_EQUIPMENT_INFO* userEquipmentInfo = connectedUser->GetUserEquipmentInfo();

	switch(Data.CARD_TYPE)
	{
	case JUNIOR_PLANE:
		if(userAccountInfo->COINS < COINS_COST_OF_JUNIOR_PLANE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_JUNIOR_PLANE;
		userEquipmentInfo->JUNIOR_PLANE_CARD_NUM = userEquipmentInfo->JUNIOR_PLANE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_JUNIOR_PLANE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM =1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "JUNIOR_PLANE", sizeof("JUNIOR_PLANE"));
		break;

	case MIDDLE_PLANE:
		if(userAccountInfo->COINS < COINS_COST_OF_MIDDLE_PLANE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_MIDDLE_PLANE;
		userEquipmentInfo->MIDDLE_PLANE_CARD_NUM = userEquipmentInfo->MIDDLE_PLANE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_MIDDLE_PLANE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "MIDDLE_PLANE", sizeof("MIDDLE_PLANE"));
		break;

	case SENIOR_PLANE:
		if(userAccountInfo->COINS < COINS_COST_OF_SENIOR_PLANE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_SENIOR_PLANE;
		userEquipmentInfo->SENIOR_PLANE_CARD_NUM = userEquipmentInfo->SENIOR_PLANE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_SENIOR_PLANE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "SENIOR_PLANE", sizeof("SENIOR_PLANE"));
		break;

	case JUNIOR_STEALTH_PLANE:
		if(userAccountInfo->COINS < COINS_COST_OF_JUNIOR_STEALTH_PLANE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_JUNIOR_STEALTH_PLANE;
		userEquipmentInfo->JUNIOR_STEALTH_PLANE_CARD_NUM = userEquipmentInfo->JUNIOR_STEALTH_PLANE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_JUNIOR_STEALTH_PLANE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "JUNIOR_STEALTH_PLANE", sizeof("JUNIOR_STEALTH_PLANE"));
		break;

	case MIDDLE_STEALTH_PLANE:
		if(userAccountInfo->COINS < COINS_COST_OF_MIDDLE_STEALTH_PLANE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		userEquipmentInfo->MIDDLE_STEALTH_PLANE_CARD_NUM = userEquipmentInfo->MIDDLE_STEALTH_PLANE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "MIDDLE_STEALTH_PLANE", sizeof("MIDDLE_STEALTH_PLANE"));
		break;

	case SENIOR_STEALTH_PLANE:
		if(userAccountInfo->COINS < COINS_COST_OF_SENIOR_STEALTH_PLANE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_SENIOR_STEALTH_PLANE;
		userEquipmentInfo->SENIOR_STEALTH_PLANE_CARD_NUM = userEquipmentInfo->SENIOR_STEALTH_PLANE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_SENIOR_STEALTH_PLANE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "SENIOR_STEALTH_PLANE", sizeof("SENIOR_STEALTH_PLANE"));
		break;

	case SENIOR_ANTI_STEALTH_RADAR:
		if(userAccountInfo->COINS < COINS_COST_OF_SENIOR_ANTI_STEALTH_RADAR) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_SENIOR_ANTI_STEALTH_RADAR;
		userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_CARD_NUM = userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_SENIOR_ANTI_STEALTH_RADAR;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "SENIOR_ANTI_STEALTH_RADAR", sizeof("SENIOR_ANTI_STEALTH_RADAR"));
		break;

	case MIDDLE_ANTI_STEALTH_RADAR:
		if(userAccountInfo->COINS < COINS_COST_OF_MIDDLE_ANTI_STEALTH_RADAR) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_MIDDLE_ANTI_STEALTH_RADAR;
		userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM = userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_MIDDLE_ANTI_STEALTH_RADAR;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "MIDDLE_ANTI_STEALTH_RADAR", sizeof("MIDDLE_ANTI_STEALTH_RADAR"));
		break;

	case JUNIOR_ANTI_STEALTH_RADAR:
		if(userAccountInfo->COINS < COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM = userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "JUNIOR_ANTI_STEALTH_RADAR", sizeof("JUNIOR_ANTI_STEALTH_RADAR"));
		break;

	case SENIOR_MISSILE:
		if(userAccountInfo->COINS < COINS_COST_OF_SENIOR_MISSILE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_SENIOR_MISSILE;
		userEquipmentInfo->SENIOR_MISSILE_CARD_NUM = userEquipmentInfo->SENIOR_MISSILE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_SENIOR_MISSILE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "SENIOR_MISSILE", sizeof("SENIOR_MISSILE"));
		break;

	case MIDDLE_MISSILE:
		if(userAccountInfo->COINS < COINS_COST_OF_MIDDLE_MISSILE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_MIDDLE_MISSILE;
		userEquipmentInfo->MIDDLE_MISSILE_CARD_NUM = userEquipmentInfo->MIDDLE_MISSILE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_MIDDLE_MISSILE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "MIDDLE_MISSILE", sizeof("MIDDLE_MISSILE"));
		break;

	case JUNIOR_MISSILE:
		if(userAccountInfo->COINS < COINS_COST_OF_JUNIOR_MISSILE) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_JUNIOR_MISSILE;
		userEquipmentInfo->JUNIOR_MISSILE_CARD_NUM = userEquipmentInfo->JUNIOR_MISSILE_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_JUNIOR_MISSILE;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "JUNIOR_MISSILE", sizeof("JUNIOR_MISSILE"));
		break;

	case SENIOR_RADAR:
		if(userAccountInfo->COINS < COINS_COST_OF_SENIOR_RADAR) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_SENIOR_RADAR;
		userEquipmentInfo->SENIOR_RADAR_CARD_NUM = userEquipmentInfo->SENIOR_RADAR_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_SENIOR_RADAR;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "SENIOR_RADAR", sizeof("SENIOR_RADAR"));
		break;

	case MIDDLE_RADAR:
		if(userAccountInfo->COINS < COINS_COST_OF_MIDDLE_RADAR) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_MIDDLE_RADAR;
		userEquipmentInfo->MIDDLE_RADAR_CARD_NUM = userEquipmentInfo->MIDDLE_RADAR_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_MIDDLE_RADAR;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "MIDDLE_RADAR", sizeof("MIDDLE_RADAR"));
		break;

	case JUNIOR_RADAR:
		if(userAccountInfo->COINS < COINS_COST_OF_JUNIOR_RADAR) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_JUNIOR_RADAR;
		userEquipmentInfo->JUNIOR_RADAR_CARD_NUM = userEquipmentInfo->JUNIOR_RADAR_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_JUNIOR_RADAR;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "JUNIOR_RADAR", sizeof("JUNIOR_RADAR"));
		break;

	case SENIOR_CANNONBALL:
		if(userAccountInfo->COINS < COINS_COST_OF_SENIOR_CANNONBALL) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_SENIOR_CANNONBALL;
		userEquipmentInfo->SENIOR_CANNONBALL_CARD_NUM = userEquipmentInfo->SENIOR_CANNONBALL_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_SENIOR_CANNONBALL;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "SENIOR_CANNONBALL", sizeof("SENIOR_CANNONBALL"));
		break;

	case MIDDLE_CANNONBALL:
		if(userAccountInfo->COINS < COINS_COST_OF_MIDDLE_CANNONBALL) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_MIDDLE_CANNONBALL;
		userEquipmentInfo->MIDDLE_CANNONBALL_CARD_NUM = userEquipmentInfo->MIDDLE_CANNONBALL_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_MIDDLE_CANNONBALL;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "MIDDLE_CANNONBALL", sizeof("MIDDLE_CANNONBALL"));
		break;

	case JUNIOR_CANNONBALL:
		if(userAccountInfo->COINS < COINS_COST_OF_JUNIOR_CANNONBALL) {
			connectedUser->WritePacket(PT_BUY_CARDS_FAIL_U, WriteBuffer, WRITE_PT_BUY_CARDS_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS -= COINS_COST_OF_JUNIOR_CANNONBALL;
		userEquipmentInfo->JUNIOR_CANNONBALL_CARD_NUM = userEquipmentInfo->JUNIOR_CANNONBALL_CARD_NUM + 1;
		ptBuyCardsSuccU.COINS = COINS_COST_OF_JUNIOR_CANNONBALL;
		ptBuyCardsSuccU.CARD_TYPE = Data.CARD_TYPE;
		ptBuyCardsSuccU.CARD_NUM = 1;
		memcpy(ptBuyCardsSuccU.CARD_NAME, "JUNIOR_CANNONBALL", sizeof("JUNIOR_CANNONBALL"));
		break;
	}

	connectedUser->WritePacket(PT_BUY_CARDS_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_BUY_CARDS_SUCC_U(
		WriteBuffer,
		ptBuyCardsSuccU));
}

VOID CGameIocp::onPT_OPEN_SUPPLY_CHEST(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_OPEN_SUPPLY_CHEST_FAIL_U);
	READ_PACKET(PT_OPEN_SUPPLY_CHEST);
	CLog::WriteLog(_T("# Read packet : PT_OPEN_SUPPLY_CHEST")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体
	
	//S_PT_GET_SUPPLY_CHEST_SUCC_U* ptGetSupplyChestSuccU = db_GetSupplyChest(Data);
	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	S_USER_EQUIPMENT_INFO* userEquipmentInfo = connectedUser->GetUserEquipmentInfo();
	S_USER_CHEST_INFO* userChestInfo = connectedUser->GetUserChestInfo();
	S_PT_OPEN_SUPPLY_CHEST_SUCC_U ptOpenSupplyChestSuccU;
	memset(&ptOpenSupplyChestSuccU, 0, sizeof(S_PT_OPEN_SUPPLY_CHEST_SUCC_U));

	int kindsOfCards = 0;
	int cardType[TOTAL_KINDS_OF_CARDS] = {0,};
	int numOfCards[TOTAL_KINDS_OF_CARDS] = {0,};
	int levelOfCards[TOTAL_KINDS_OF_CARDS] = {0,};
	//char nameOfCards[32][TOTAL_KINDS_OF_CARDS] = {0,};
	
	switch(Data.CHEST_TYPE)
	{
	case WIN_REWARDS_1:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_WIN_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_WIN_REWARDS;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_WIN_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_WIN_REWARDS;
		userChestInfo->WIN_REWARDS_1_NUM -= 1;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->WIN_REWARDS_1_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = WIN_REWARDS_1;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_WIN_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_WIN_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_WIN_REWARDS, MOST_CARDS_IN_WIN_REWARDS, connectedUser);
		break;

	case WIN_REWARDS_2:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_WIN_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_WIN_REWARDS;
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_WIN_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_WIN_REWARDS;
		userChestInfo->WIN_REWARDS_2_NUM -= 1;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->WIN_REWARDS_2_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = WIN_REWARDS_2;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_WIN_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_WIN_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_WIN_REWARDS, MOST_CARDS_IN_WIN_REWARDS, connectedUser);
		break;

	case WIN_REWARDS_3:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_WIN_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_WIN_REWARDS; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_WIN_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_WIN_REWARDS;
		userChestInfo->WIN_REWARDS_3_NUM -= 1;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->WIN_REWARDS_3_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = WIN_REWARDS_3;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_WIN_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_WIN_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_WIN_REWARDS, MOST_CARDS_IN_WIN_REWARDS, connectedUser);
		break;

	case WIN_REWARDS_4:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_WIN_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_WIN_REWARDS; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_WIN_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_WIN_REWARDS;
		userChestInfo->WIN_REWARDS_4_NUM -= 1;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->WIN_REWARDS_4_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = WIN_REWARDS_4;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_WIN_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_WIN_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_WIN_REWARDS, MOST_CARDS_IN_WIN_REWARDS, connectedUser);
		break;

	case WIN_REWARDS_5:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_WIN_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_WIN_REWARDS; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_WIN_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_WIN_REWARDS;
		userChestInfo->WIN_REWARDS_5_NUM -= 1;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->WIN_REWARDS_5_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = WIN_REWARDS_5;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_WIN_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_WIN_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_WIN_REWARDS, MOST_CARDS_IN_WIN_REWARDS, connectedUser);
		break;

	case STAR_REWARDS:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_STAR_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_STAR_REWARDS; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_STAR_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_STAR_REWARDS;
		userChestInfo->STAR_NUM = 0;
		userChestInfo->STAR_REWARDS_NUM -= 1;
		ptOpenSupplyChestSuccU.STAR_NUM = 0;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->STAR_REWARDS_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = STAR_REWARDS;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_STAR_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_STAR_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_STAR_REWARDS, MOST_CARDS_IN_STAR_REWARDS, connectedUser);
		break;

	case FREE_REWARDS:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_FREE_REWARDS) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_FREE_REWARDS; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_FREE_REWARDS;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_FREE_REWARDS;
		userChestInfo->FREE_REWARDS_NUM -= 1;
		ptOpenSupplyChestSuccU.CHEST_NUM = userChestInfo->FREE_REWARDS_NUM;
		ptOpenSupplyChestSuccU.CHEST_TYPE = FREE_REWARDS;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_FREE_REWARDS;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_FREE_REWARDS;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_FREE_REWARDS, MOST_CARDS_IN_FREE_REWARDS, connectedUser);
		break;

	case SUPPLY_CHEST_SMALL:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_SUPPLY_CHEST_SMALL) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_SUPPLY_CHEST_SMALL; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_SUPPLY_CHEST_SMALL;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_SUPPLY_CHEST_SMALL;
		ptOpenSupplyChestSuccU.CHEST_TYPE = SUPPLY_CHEST_SMALL;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_SUPPLY_CHEST_SMALL;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_SUPPLY_CHEST_SMALL;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_SUPPLY_CHEST_SMALL, MOST_CARDS_IN_SUPPLY_CHEST_SMALL, connectedUser);
		break;

	case SUPPLY_CHEST_MIDDLE:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_SUPPLY_CHEST_MIDDLE) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_SUPPLY_CHEST_MIDDLE; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_SUPPLY_CHEST_MIDDLE;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_SUPPLY_CHEST_MIDDLE;
		ptOpenSupplyChestSuccU.CHEST_TYPE = SUPPLY_CHEST_MIDDLE;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_SUPPLY_CHEST_MIDDLE;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_SUPPLY_CHEST_MIDDLE;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_SUPPLY_CHEST_MIDDLE, MOST_CARDS_IN_SUPPLY_CHEST_MIDDLE, connectedUser);
		break;

	case SUPPLY_CHEST_LARGE:
		if(userAccountInfo->DIAMONDS < DIAMONDS_COST_FOR_SUPPLY_CHEST_LARGE) {
			connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_FAIL_U, WriteBuffer, WRITE_PT_OPEN_SUPPLY_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userAccountInfo->COINS = userAccountInfo->COINS + COINS_IN_SUPPLY_CHEST_LARGE; 
		userAccountInfo->DIAMONDS -= DIAMONDS_COST_FOR_SUPPLY_CHEST_LARGE;
		userAccountInfo->DIAMONDS += DIAMONDS_IN_SUPPLY_CHEST_LARGE;
		ptOpenSupplyChestSuccU.CHEST_TYPE = SUPPLY_CHEST_LARGE;
		ptOpenSupplyChestSuccU.COINS = COINS_IN_SUPPLY_CHEST_LARGE;
		ptOpenSupplyChestSuccU.DIAMONDS = DIAMONDS_IN_SUPPLY_CHEST_LARGE;

		// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
		kindsOfCards = assignCards(cardType, numOfCards, levelOfCards, KINDS_OF_CARD_IN_SUPPLY_CHEST_LARGE, MOST_CARDS_IN_SUPPLY_CHEST_LARGE, connectedUser);
		break;
	}

	// 将分配好的卡牌装入协议结构体 ptGetSupplyChestSuccU 准备发送
	loadCardsInSupplyChest(&ptOpenSupplyChestSuccU, cardType, numOfCards, levelOfCards, /*(char*) nameOfCards,*/ kindsOfCards);

	connectedUser->WritePacket(PT_OPEN_SUPPLY_CHEST_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_OPEN_SUPPLY_CHEST_SUCC_U(
		WriteBuffer,
		ptOpenSupplyChestSuccU));
}

// 分配补给箱中的卡牌 同时更改 userEquipmentInfo 中对应卡牌数量
int CGameIocp::assignCards(int* cardType, int* numOfCards, int* levelOfCards, /*char* nameOfCards, */int kindsOfCards, int rangeOfCardNum, CConnectedUser* connectedUser)
{
	CRandom random;
	int CardType[TOTAL_KINDS_OF_CARDS];
	int NumOfCards[TOTAL_KINDS_OF_CARDS];
	int LevelOfCards[TOTAL_KINDS_OF_CARDS];
	//char NameOfCards[TOTAL_KINDS_OF_CARDS][32];

	S_USER_EQUIPMENT_INFO* userEquipmentInfo = connectedUser->GetUserEquipmentInfo();

	random.Init(time(NULL));

	for(int i = 0; i < kindsOfCards; i++)
	{	 
		bool equal = true;
		while(equal)
		{
			equal = false;
			CardType[i] = JUNIOR_PLANE + random.getRandomWithTime(TOTAL_KINDS_OF_CARDS);

			for(int j = 0; j < i; j++)
			{
				if(CardType[j] == CardType[i]) {equal = true; break;}
			}
		}

		switch(CardType[i])
		{
		case JUNIOR_PLANE:
			//sprintf(NameOfCards[i], "JUNIOR_PLANE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->JUNIOR_PLANE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->JUNIOR_PLANE_LEVEL == 0) {
				userEquipmentInfo->JUNIOR_PLANE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->JUNIOR_PLANE_LEVEL;
			break;

		case MIDDLE_PLANE:
			//sprintf(NameOfCards[i], "MIDDLE_PLANE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->MIDDLE_PLANE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->MIDDLE_PLANE_LEVEL == 0) {
				userEquipmentInfo->MIDDLE_PLANE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->MIDDLE_PLANE_LEVEL;
			break;

		case SENIOR_PLANE:
			//sprintf(NameOfCards[i], "SENIOR_PLANE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->SENIOR_PLANE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->SENIOR_PLANE_LEVEL == 0) {
				userEquipmentInfo->SENIOR_PLANE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->SENIOR_PLANE_LEVEL;
			break;

		case JUNIOR_STEALTH_PLANE:
			//sprintf(NameOfCards[i], "JUNIOR_STEALTH_PLANE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->JUNIOR_STEALTH_PLANE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->JUNIOR_STEALTH_PLANE_LEVEL == 0) {
				userEquipmentInfo->JUNIOR_STEALTH_PLANE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->JUNIOR_STEALTH_PLANE_LEVEL;
			break;

		case MIDDLE_STEALTH_PLANE:
			//sprintf(NameOfCards[i], "MIDDLE_STEALTH_PLANE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->MIDDLE_STEALTH_PLANE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->MIDDLE_STEALTH_PLANE_LEVEL == 0) {
				userEquipmentInfo->MIDDLE_STEALTH_PLANE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->MIDDLE_STEALTH_PLANE_LEVEL;
			break;

		case SENIOR_STEALTH_PLANE:
			//sprintf(NameOfCards[i], "SENIOR_STEALTH_PLANE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->SENIOR_STEALTH_PLANE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->SENIOR_STEALTH_PLANE_LEVEL == 0) {
				userEquipmentInfo->SENIOR_STEALTH_PLANE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->SENIOR_STEALTH_PLANE_LEVEL;
			break;

		case SENIOR_ANTI_STEALTH_RADAR:
			//sprintf(NameOfCards[i], "SENIOR_ANTI_STEALTH_RADAR");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_LEVEL == 0) {
				userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_LEVEL;
			break;

		case MIDDLE_ANTI_STEALTH_RADAR:
			//sprintf(NameOfCards[i], "MIDDLE_ANTI_STEALTH_RADAR");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_LEVEL == 0) {
				userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_LEVEL;
			break;

		case JUNIOR_ANTI_STEALTH_RADAR:
			//sprintf(NameOfCards[i], "JUNIOR_ANTI_STEALTH_RADAR");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_LEVEL == 0) {
				userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_LEVEL;
			break;

		case SENIOR_MISSILE:
			//sprintf(NameOfCards[i], "SENIOR_MISSILE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->SENIOR_MISSILE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->SENIOR_MISSILE_LEVEL == 0) {
				userEquipmentInfo->SENIOR_MISSILE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->SENIOR_MISSILE_LEVEL;
			break;

		case MIDDLE_MISSILE:
			//sprintf(NameOfCards[i], "MIDDLE_MISSILE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->MIDDLE_MISSILE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->MIDDLE_MISSILE_LEVEL == 0) {
				userEquipmentInfo->MIDDLE_MISSILE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->MIDDLE_MISSILE_LEVEL;
			break;

		case JUNIOR_MISSILE:
			//sprintf(NameOfCards[i], "JUNIOR_MISSILE");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->JUNIOR_MISSILE_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->JUNIOR_MISSILE_LEVEL == 0) {
				userEquipmentInfo->JUNIOR_MISSILE_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->JUNIOR_MISSILE_LEVEL;
			break;

		case SENIOR_RADAR:
			//sprintf(NameOfCards[i], "SENIOR_RADAR");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->SENIOR_RADAR_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->SENIOR_RADAR_LEVEL == 0) {
				userEquipmentInfo->SENIOR_RADAR_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->SENIOR_RADAR_LEVEL;
			break;

		case MIDDLE_RADAR:
			//sprintf(NameOfCards[i], "MIDDLE_RADAR");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->MIDDLE_RADAR_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->MIDDLE_RADAR_LEVEL == 0) {
				userEquipmentInfo->MIDDLE_RADAR_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->MIDDLE_RADAR_LEVEL;
			break;

		case JUNIOR_RADAR:
			//sprintf(NameOfCards[i], "JUNIOR_RADAR");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->JUNIOR_RADAR_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->JUNIOR_RADAR_LEVEL == 0) {
				userEquipmentInfo->JUNIOR_RADAR_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->JUNIOR_RADAR_LEVEL;
			break;

		case SENIOR_CANNONBALL:
			//sprintf(NameOfCards[i], "SENIOR_CANNONBALL");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->SENIOR_CANNONBALL_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->SENIOR_CANNONBALL_LEVEL == 0) {
				userEquipmentInfo->SENIOR_CANNONBALL_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->SENIOR_CANNONBALL_LEVEL;
			break;

		case MIDDLE_CANNONBALL:
			//sprintf(NameOfCards[i], "MIDDLE_CANNONBALL");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->MIDDLE_CANNONBALL_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->MIDDLE_CANNONBALL_LEVEL == 0) {
				userEquipmentInfo->MIDDLE_CANNONBALL_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->MIDDLE_CANNONBALL_LEVEL;
			break;

		case JUNIOR_CANNONBALL:
			//sprintf(NameOfCards[i], "JUNIOR_CANNONBALL");
			NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum) + 1;
			userEquipmentInfo->JUNIOR_CANNONBALL_CARD_NUM += NumOfCards[i];
			if(userEquipmentInfo->JUNIOR_CANNONBALL_LEVEL == 0) {
				userEquipmentInfo->JUNIOR_CANNONBALL_LEVEL = 1;
			}
			LevelOfCards[i] = userEquipmentInfo->JUNIOR_CANNONBALL_LEVEL;
			break;
		}
	}

	memcpy(cardType, CardType, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	memcpy(numOfCards, NumOfCards, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	memcpy(levelOfCards, LevelOfCards, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	//memcpy(nameOfCards, NameOfCards, sizeof(char)*32*TOTAL_KINDS_OF_CARDS);

	return kindsOfCards;
}

// 将分配好的卡牌装入协议结构体 ptGetSupplyChestSuccU 准备发送
void CGameIocp::loadCardsInSupplyChest(S_PT_OPEN_SUPPLY_CHEST_SUCC_U* ptOpenSupplyChestSuccU, int* cardType, int* numOfCards, int* levelOfCards, /*char* nameOfCards,*/ int kindsOfCards)
{

	int CardType[TOTAL_KINDS_OF_CARDS] = {0,};
	int NumOfCards[TOTAL_KINDS_OF_CARDS] = {0,};
	int LevelOfCards[TOTAL_KINDS_OF_CARDS] = {0,};
	//char NameOfCards[TOTAL_KINDS_OF_CARDS][32] = {0,};

	memcpy(CardType, cardType, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	memcpy(NumOfCards, numOfCards, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	memcpy(LevelOfCards, levelOfCards, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	//memcpy(NameOfCards, nameOfCards, sizeof(char)*32*TOTAL_KINDS_OF_CARDS);

	ptOpenSupplyChestSuccU->CARD_1_TYPE = CardType[0];
	ptOpenSupplyChestSuccU->CARD_1_NUM = NumOfCards[0];
	ptOpenSupplyChestSuccU->CARD_1_LEVEL = LevelOfCards[0];
	//memcpy(&ptGetSupplyChestSuccU->CARD_1_NAME, NameOfCards[0], sizeof(char)*32);
	if(kindsOfCards == 1) return;

	ptOpenSupplyChestSuccU->CARD_2_TYPE = CardType[1];
	ptOpenSupplyChestSuccU->CARD_2_NUM = NumOfCards[1];
	ptOpenSupplyChestSuccU->CARD_2_LEVEL = LevelOfCards[1];
	//memcpy(&ptGetSupplyChestSuccU->CARD_2_NAME, NameOfCards[1], sizeof(char)*32);
	if(kindsOfCards == 2) return;

	ptOpenSupplyChestSuccU->CARD_3_TYPE = CardType[2];
	ptOpenSupplyChestSuccU->CARD_3_NUM = NumOfCards[2];
	ptOpenSupplyChestSuccU->CARD_3_LEVEL = LevelOfCards[2];
	//memcpy(&ptGetSupplyChestSuccU->CARD_3_NAME, NameOfCards[2], sizeof(char)*32);
	if(kindsOfCards == 3) return;

	ptOpenSupplyChestSuccU->CARD_4_TYPE = CardType[3];
	ptOpenSupplyChestSuccU->CARD_4_NUM = NumOfCards[3];
	ptOpenSupplyChestSuccU->CARD_4_LEVEL = LevelOfCards[3];
	//memcpy(&ptGetSupplyChestSuccU->CARD_4_NAME, NameOfCards[3], sizeof(char)*32);
	if(kindsOfCards == 4) return;

	ptOpenSupplyChestSuccU->CARD_5_TYPE = CardType[4];
	ptOpenSupplyChestSuccU->CARD_5_NUM = NumOfCards[4];
	ptOpenSupplyChestSuccU->CARD_5_LEVEL = LevelOfCards[4];
	//memcpy(&ptGetSupplyChestSuccU->CARD_5_NAME, NameOfCards[4], sizeof(char)*32);
	if(kindsOfCards == 5) return;

	ptOpenSupplyChestSuccU->CARD_6_TYPE = CardType[5];
	ptOpenSupplyChestSuccU->CARD_6_NUM = NumOfCards[5];
	ptOpenSupplyChestSuccU->CARD_6_LEVEL = LevelOfCards[5];
	//memcpy(&ptGetSupplyChestSuccU->CARD_6_NAME, NameOfCards[5], sizeof(char)*32);
	if(kindsOfCards == 6) return;

	ptOpenSupplyChestSuccU->CARD_7_TYPE = CardType[6];
	ptOpenSupplyChestSuccU->CARD_7_NUM = NumOfCards[6];
	ptOpenSupplyChestSuccU->CARD_7_LEVEL = LevelOfCards[6];
	//memcpy(&ptGetSupplyChestSuccU->CARD_7_NAME, NameOfCards[6], sizeof(char)*32);
	if(kindsOfCards == 7) return;

	ptOpenSupplyChestSuccU->CARD_8_TYPE = CardType[7];
	ptOpenSupplyChestSuccU->CARD_8_NUM = NumOfCards[7];
	ptOpenSupplyChestSuccU->CARD_8_LEVEL = LevelOfCards[7];
	//memcpy(&ptGetSupplyChestSuccU->CARD_8_NAME, NameOfCards[7], sizeof(char)*32);
	return;
}

VOID CGameIocp::onPT_ACQUIRE_CHEST(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_ACQUIRE_CHEST_FAIL_U);
	READ_PACKET(PT_ACQUIRE_CHEST);
	CLog::WriteLog(_T("# Read packet : PT_ACQUIRE_CHEST")); // 此处的 Data 为接收客户端传来数据解析后得到的对应协议结构体
	
	//S_PT_GET_SUPPLY_CHEST_SUCC_U* ptGetSupplyChestSuccU = db_GetSupplyChest(Data);
	S_USER_ACCOUNT_INFO* userAccountInfo = connectedUser->GetUserAccountInfo();
	S_USER_EQUIPMENT_INFO* userEquipmentInfo = connectedUser->GetUserEquipmentInfo();
	S_USER_CHEST_INFO* userChestInfo = connectedUser->GetUserChestInfo();

	INT64 currentTime;
	time((time_t*)&currentTime);

	S_PT_ACQUIRE_CHEST_SUCC_U ptAcquireChestSuccU;
	memset(&ptAcquireChestSuccU, 0, sizeof(S_PT_ACQUIRE_CHEST_SUCC_U));
	
	switch(Data.CHEST_TYPE)
	{
	case 	WIN_REWARDS_1:
		if(userChestInfo->WIN_REWARDS_1_NUM > 0) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userChestInfo->WIN_REWARDS_1_NUM = 1;
		userChestInfo->WIN_REWARDS_1_UNLOCK_TIME = currentTime + WIN_REWARDS_UNLOCK_DURATION;
		ptAcquireChestSuccU.CHEST_TYPE = WIN_REWARDS_1;
		ptAcquireChestSuccU.UNLOCK_TIME = userChestInfo->WIN_REWARDS_1_UNLOCK_TIME;
		break;

	case WIN_REWARDS_2:
		if(userChestInfo->WIN_REWARDS_2_NUM > 0) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userChestInfo->WIN_REWARDS_2_NUM = 1;
		userChestInfo->WIN_REWARDS_2_UNLOCK_TIME = currentTime + WIN_REWARDS_UNLOCK_DURATION;
		ptAcquireChestSuccU.CHEST_TYPE = WIN_REWARDS_2;
		ptAcquireChestSuccU.UNLOCK_TIME = userChestInfo->WIN_REWARDS_2_UNLOCK_TIME;
		break;

	case WIN_REWARDS_3:
		if(userChestInfo->WIN_REWARDS_3_NUM > 0) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userChestInfo->WIN_REWARDS_3_NUM = 1;
		time((time_t*)&userChestInfo->WIN_REWARDS_3_UNLOCK_TIME);
		userChestInfo->WIN_REWARDS_3_UNLOCK_TIME = currentTime + WIN_REWARDS_UNLOCK_DURATION;
		ptAcquireChestSuccU.CHEST_TYPE = WIN_REWARDS_3;
		ptAcquireChestSuccU.UNLOCK_TIME = userChestInfo->WIN_REWARDS_3_UNLOCK_TIME;
		break;

	case WIN_REWARDS_4:
		if(userChestInfo->WIN_REWARDS_4_NUM) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userChestInfo->WIN_REWARDS_4_NUM = 1;
		time((time_t*)&userChestInfo->WIN_REWARDS_4_UNLOCK_TIME);
		userChestInfo->WIN_REWARDS_4_UNLOCK_TIME = currentTime + WIN_REWARDS_UNLOCK_DURATION;
		ptAcquireChestSuccU.CHEST_TYPE = WIN_REWARDS_4;
		ptAcquireChestSuccU.UNLOCK_TIME = userChestInfo->WIN_REWARDS_4_UNLOCK_TIME;
		break;

	case WIN_REWARDS_5:
		if(userChestInfo->WIN_REWARDS_5_NUM) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}
		userChestInfo->WIN_REWARDS_5_NUM = 1;
		time((time_t*)&userChestInfo->WIN_REWARDS_5_UNLOCK_TIME);
		userChestInfo->WIN_REWARDS_5_UNLOCK_TIME = currentTime + WIN_REWARDS_UNLOCK_DURATION;
		ptAcquireChestSuccU.CHEST_TYPE = WIN_REWARDS_5;
		ptAcquireChestSuccU.UNLOCK_TIME = userChestInfo->WIN_REWARDS_5_UNLOCK_TIME;
		break;

	case STAR_REWARDS:
		if(currentTime - userChestInfo->LAST_STAR_REWARDS_ACQUIRE_TIME  < STAR_REWARDS_INTERVAL) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}  else if(userChestInfo->STAR_NUM < 15) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		} else if(userChestInfo->STAR_REWARDS_NUM >= 1) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}

		userChestInfo->STAR_NUM = 0;
		userChestInfo->STAR_REWARDS_NUM = 1;
		userChestInfo->LAST_STAR_REWARDS_ACQUIRE_TIME = currentTime;
		ptAcquireChestSuccU.CHEST_TYPE = STAR_REWARDS;
		ptAcquireChestSuccU.ACQUIRE_TIME = userChestInfo->LAST_STAR_REWARDS_ACQUIRE_TIME;
		break;

	case FREE_REWARDS:
		if(currentTime - userChestInfo->LAST_FREE_REWARDS_ACQUIRE_TIME < FREE_REWARDS_INTERVAL) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		} else if(userChestInfo->FREE_REWARDS_NUM >= 1) {
			connectedUser->WritePacket(PT_ACQUIRE_CHEST_FAIL_U, WriteBuffer, WRITE_PT_ACQUIRE_CHEST_FAIL_U(WriteBuffer, 1));
			return;
		}

		userChestInfo->FREE_REWARDS_NUM = 1;
		userChestInfo->LAST_FREE_REWARDS_ACQUIRE_TIME = currentTime;
		ptAcquireChestSuccU.CHEST_TYPE = FREE_REWARDS;
		ptAcquireChestSuccU.ACQUIRE_TIME = userChestInfo->LAST_FREE_REWARDS_ACQUIRE_TIME;
		break;
	}

	connectedUser->WritePacket(PT_ACQUIRE_CHEST_SUCC_U, 
		WriteBuffer, 
		WRITE_PT_ACQUIRE_CHEST_SUCC_U(
		WriteBuffer,
		ptAcquireChestSuccU));

	return;
}

























