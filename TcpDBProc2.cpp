#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"

#include "../Packet/ErrorCode.h"
#include "../Packet/PURCHASE_PT_ConstValue.h"
//#include "../Packet/UserProtocol.h"
//#include "../Packet/User_PT_Structure.h"
//#include "../Packet/User_PT_ReadPacket.h"
//#include "../Packet/User_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"
#include "../LowLib/Random.h"

S_PT_BUY_COINS_SUCC_U* CGameIocp::db_BuyCoins(S_PT_BUY_COINS ptBuyCoins)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_BUY_COINS_SUCC_U *ptBuyCoinsSuccU = new S_PT_BUY_COINS_SUCC_U();
	memset(ptBuyCoinsSuccU, 0, sizeof(S_PT_BUY_COINS_SUCC_U));

	switch(ptBuyCoins.PACKET_LEVEL)
	{
	case PACKET_LEVEL_1:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_1;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_1;
		break;
	case PACKET_LEVEL_2:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_2;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_2;
		break;
	case PACKET_LEVEL_3:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_3;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_3;
		break;
	case PACKET_LEVEL_4:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_4;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_4;
		break;
	case PACKET_LEVEL_5:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_5;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_5;
		break;
	case PACKET_LEVEL_6:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_6;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_6;
		break;
	case PACKET_LEVEL_7:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_7;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_7;
		break;
	case PACKET_LEVEL_8:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_8;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_8;
		break;
	case PACKET_LEVEL_9:
		ptBuyCoins.COINS = ptBuyCoins.COINS + COINS_PACKET_LEVEL_9;
		ptBuyCoins.DIAMONDS = ptBuyCoins.DIAMONDS - DIAMONDS_COST_OF_COINS_PACKET_LEVEL_9;
		break;
	}

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = %d and %s = '%s'", USER_TABLE_NAME, "COINS", ptBuyCoins.COINS,"USER_ID", ptBuyCoins.USER_ID, "DIAMONDS", ptBuyCoins.DIAMONDS, "PASSWORD", ptBuyCoins.PASSWORD);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	ptBuyCoinsSuccU->PACKET_LEVEL = ptBuyCoins.PACKET_LEVEL;
	ptBuyCoinsSuccU->COINS = ptBuyCoins.COINS;
	ptBuyCoinsSuccU->DIAMONDS = ptBuyCoins.DIAMONDS;

	mySqlSession->FreeSession();
	
	return ptBuyCoinsSuccU;
}

S_PT_BUY_DIAMONDS_SUCC_U* CGameIocp::db_BuyDiamonds(S_PT_BUY_DIAMONDS ptBuyDiamonds)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_BUY_DIAMONDS_SUCC_U *ptBuyDiamondsSuccU = new S_PT_BUY_DIAMONDS_SUCC_U();
	memset(ptBuyDiamondsSuccU, 0, sizeof(S_PT_BUY_DIAMONDS_SUCC_U));

	switch(ptBuyDiamonds.PACKET_LEVEL)
	{
	case PACKET_LEVEL_1:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_1;
		break;
	case PACKET_LEVEL_2:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_2;
		break;
	case PACKET_LEVEL_3:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_3;
		break;
	case PACKET_LEVEL_4:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_4;
		break;
	case PACKET_LEVEL_5:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_5;
		break;
	case PACKET_LEVEL_6:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_6;
		break;
	case PACKET_LEVEL_7:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_7;
		break;
	case PACKET_LEVEL_8:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_8;
		break;
	case PACKET_LEVEL_9:
		ptBuyDiamonds.DIAMONDS = ptBuyDiamonds.DIAMONDS + DIAMONDS_PACKET_LEVEL_9;
		break;
	}

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", USER_TABLE_NAME, "DIAMONDS", ptBuyDiamonds.DIAMONDS,"USER_ID", ptBuyDiamonds.USER_ID, "PASSWORD", ptBuyDiamonds.PASSWORD);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	ptBuyDiamondsSuccU->PACKET_LEVEL = ptBuyDiamonds.PACKET_LEVEL;
	ptBuyDiamondsSuccU->DIAMONDS = ptBuyDiamonds.DIAMONDS;

	mySqlSession->FreeSession();
	
	return ptBuyDiamondsSuccU;
}

S_PT_BUY_CARDS_SUCC_U* CGameIocp::db_BuyCards(S_PT_BUY_CARDS ptBuyCards)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();
	S_PT_BUY_CARDS_SUCC_U *ptBuyCardsSuccU = new S_PT_BUY_CARDS_SUCC_U();
	memset(ptBuyCardsSuccU, 0, sizeof(S_PT_BUY_CARDS_SUCC_U));

	ptBuyCards.CARD_NUM = ptBuyCards.CARD_NUM + 1;

	switch(ptBuyCards.CARD_TYPE)
	{
	case JUNIOR_PLANE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_JUNIOR_PLANE;
		break;
	case MIDDLE_PLANE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_MIDDLE_PLANE;
		break;
	case SENIOR_PLANE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_SENIOR_PLANE;
		break;
	case JUNIOR_STEALTH_PLANE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_JUNIOR_STEALTH_PLANE;
		break;
	case MIDDLE_STEALTH_PLANE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_MIDDLE_STEALTH_PLANE;
		break;
	case SENIOR_STEALTH_PLANE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_SENIOR_STEALTH_PLANE;
		break;
	case SENIOR_ANTI_STEALTH_RADAR:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_SENIOR_ANTI_STEALTH_RADAR;
		break;
	case MIDDLE_ANTI_STEALTH_RADAR:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_MIDDLE_ANTI_STEALTH_RADAR;
		break;
	case JUNIOR_ANTI_STEALTH_RADAR:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_JUNIOR_ANTI_STEALTH_RADAR;
		break;
	case SENIOR_MISSILE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_SENIOR_MISSILE;
		break;
	case MIDDLE_MISSILE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_MIDDLE_MISSILE;
		break;
	case JUNIOR_MISSILE:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_JUNIOR_MISSILE;
		break;
	case SENIOR_RADAR:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_SENIOR_RADAR;
		break;
	case MIDDLE_RADAR:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_MIDDLE_RADAR;
		break;
	case JUNIOR_RADAR:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_JUNIOR_RADAR;
		break;
	case SENIOR_CANNONBALL:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_SENIOR_CANNONBALL;
		break;
	case MIDDLE_CANNONBALL:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_MIDDLE_CANNONBALL;
		break;
	case JUNIOR_CANNONBALL:
		ptBuyCards.COINS = ptBuyCards.COINS - COINS_COST_OF_JUNIOR_CANNONBALL;
		break;
	}

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptBuyCards.CARD_NAME, ptBuyCards.CARD_NUM,"USER_ID", ptBuyCards.USER_ID, "PASSWORD", ptBuyCards.PASSWORD);

	if(!mySqlSession->OPERATE_DATA())
		return NULL;

	ptBuyCardsSuccU->CARD_NUM = ptBuyCards.CARD_NUM;
	ptBuyCardsSuccU->CARD_TYPE = ptBuyCards.CARD_TYPE;
	memcpy(&ptBuyCardsSuccU->CARD_NAME, &ptBuyCards.CARD_NAME, sizeof(ptBuyCards.CARD_NAME));
	ptBuyCardsSuccU->COINS = ptBuyCards.COINS;

	mySqlSession->FreeSession();
	
	return ptBuyCardsSuccU;
}

S_PT_GET_SUPPLY_CHEST_SUCC_U* CGameIocp::db_GetSupplyChest(S_PT_GET_SUPPLY_CHEST ptGetSupplyChest)
{
	S_PT_GET_SUPPLY_CHEST_SUCC_U *ptGetSupplyChestSuccU = new S_PT_GET_SUPPLY_CHEST_SUCC_U();
	memset(ptGetSupplyChestSuccU, 0, sizeof(S_PT_GET_SUPPLY_CHEST_SUCC_U));

	int kindsOfCards = 0;
	int cardType[TOTAL_KINDS_OF_CARDS] = {0,};
	int numOfCards[TOTAL_KINDS_OF_CARDS] = {0,};
	char nameOfCards[32][TOTAL_KINDS_OF_CARDS] = {0,};

	switch(ptGetSupplyChest.CHEST_TYPE)
	{
	case SUPPLY_CHEST_TYPE_1:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_1; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_1;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_1, MOST_CARDS_IN_CHEST_TYPE_1);
		break;
	case SUPPLY_CHEST_TYPE_2:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_2; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_2;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_2, MOST_CARDS_IN_CHEST_TYPE_2);
		break;
	case SUPPLY_CHEST_TYPE_3:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_3; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_3;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_3, MOST_CARDS_IN_CHEST_TYPE_3);
		break;
	case SUPPLY_CHEST_TYPE_4:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_4; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_4;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_4, MOST_CARDS_IN_CHEST_TYPE_4);
		break;
	case SUPPLY_CHEST_TYPE_5:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_5; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_5;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_5, MOST_CARDS_IN_CHEST_TYPE_5);
		break;
	case SUPPLY_CHEST_TYPE_6:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_6; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_6;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_6, MOST_CARDS_IN_CHEST_TYPE_6);
		break;
	case SUPPLY_CHEST_TYPE_7:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_7; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_7;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_7, MOST_CARDS_IN_CHEST_TYPE_7);
		break;
	case SUPPLY_CHEST_TYPE_8:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_8; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_8;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_8, MOST_CARDS_IN_CHEST_TYPE_8);
		break;
	case SUPPLY_CHEST_TYPE_9:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_CHEST_TYPE_9; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_CHEST_TYPE_9;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_CHEST_TYPE_9, MOST_CARDS_IN_CHEST_TYPE_9);
		break;
	case SUPPLY_CHEST_FREE:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_FREE_CHEST; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_FREE_CHEST;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_FREE_CHEST, MOST_CARDS_IN_FREE_CHEST);
		break;
	case BATTLE_REWARDS:
		ptGetSupplyChest.COINS = ptGetSupplyChest.COINS + COINS_IN_BATTLE_REWARDS; 
		ptGetSupplyChest.DIAMONDS = ptGetSupplyChest.DIAMONDS + DIAMONDS_IN_BATTLE_REWARDS;
		kindsOfCards = assignCards(cardType, numOfCards, (char*)nameOfCards, KINDS_OF_CARD_IN_BATTLE_REWARDS, MOST_CARDS_IN_BATTLE_REWARDS);
	}

	ptGetSupplyChestSuccU->COINS = ptGetSupplyChest.COINS;
	ptGetSupplyChestSuccU->DIAMONDS = ptGetSupplyChest.DIAMONDS;
	loadCardsInSupplyChest(ptGetSupplyChestSuccU, cardType, numOfCards, (char*)nameOfCards, kindsOfCards);

	if(!updateDBwithDataInSupplyChest(ptGetSupplyChestSuccU,ptGetSupplyChest.USER_ID, ptGetSupplyChest.PASSWORD, kindsOfCards)) {
		return NULL;
	}

	return ptGetSupplyChestSuccU;
}

int CGameIocp::assignCards(int* cardType, int* numOfCards, char* nameOfCards, int kindsOfCards, int rangeOfCardNum)
{
	CRandom random;
	int CardType[TOTAL_KINDS_OF_CARDS];
	int NumOfCards[TOTAL_KINDS_OF_CARDS];
	char NameOfCards[32][TOTAL_KINDS_OF_CARDS];

	random.Init(time(NULL));
	//int cardType[TOTAL_KINDS_OF_CARDS];

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
			sprintf(NameOfCards[i], "JUNIOR_PLANE");
			break;
		case MIDDLE_PLANE:
			sprintf(NameOfCards[i], "MIDDLE_PLANE");
			break;
		case SENIOR_PLANE:
			sprintf(NameOfCards[i], "SENIOR_PLANE");
			break;
		case JUNIOR_STEALTH_PLANE:
			sprintf(NameOfCards[i], "JUNIOR_STEALTH_PLANE");
			break;
		case MIDDLE_STEALTH_PLANE:
			sprintf(NameOfCards[i], "MIDDLE_STEALTH_PLANE");
			break;
		case SENIOR_STEALTH_PLANE:
			sprintf(NameOfCards[i], "SENIOR_STEALTH_PLANE");
			break;
		case SENIOR_ANTI_STEALTH_RADAR:
			sprintf(NameOfCards[i], "SENIOR_ANTI_STEALTH_RADAR");
			break;
		case MIDDLE_ANTI_STEALTH_RADAR:
			sprintf(NameOfCards[i], "MIDDLE_ANTI_STEALTH_RADAR");
			break;
		case JUNIOR_ANTI_STEALTH_RADAR:
			sprintf(NameOfCards[i], "JUNIOR_ANTI_STEALTH_RADAR");
			break;
		case SENIOR_MISSILE:
			sprintf(NameOfCards[i], "SENIOR_MISSILE");
			break;
		case MIDDLE_MISSILE:
			sprintf(NameOfCards[i], "MIDDLE_MISSILE");
			break;
		case JUNIOR_MISSILE:
			sprintf(NameOfCards[i], "JUNIOR_MISSILE");
			break;
		case SENIOR_RADAR:
			sprintf(NameOfCards[i], "SENIOR_RADAR");
			break;
		case MIDDLE_RADAR:
			sprintf(NameOfCards[i], "MIDDLE_RADAR");
			break;
		case JUNIOR_RADAR:
			sprintf(NameOfCards[i], "JUNIOR_RADAR");
			break;
		case SENIOR_CANNONBALL:
			sprintf(NameOfCards[i], "SENIOR_CANNONBALL");
			break;
		case MIDDLE_CANNONBALL:
			sprintf(NameOfCards[i], "MIDDLE_CANNONBALL");
			break;
		case JUNIOR_CANNONBALL:
			sprintf(NameOfCards[i], "JUNIOR_CANNONBALL");
			break;
		}
		NumOfCards[i] = random.getRandomWithTime(rangeOfCardNum);
	}

	memcpy(cardType, CardType, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	memcpy(numOfCards, NumOfCards, sizeof(int)*TOTAL_KINDS_OF_CARDS);
	memcpy(nameOfCards, NameOfCards, sizeof(char)*32*TOTAL_KINDS_OF_CARDS);

	return kindsOfCards;
}

void CGameIocp::loadCardsInSupplyChest(S_PT_GET_SUPPLY_CHEST_SUCC_U* ptGetSupplyChestSuccU, int* cardType, int* numOfCards, char* nameOfCards, int kindsOfCards)
{
	char* pointerForNameOfCard = nameOfCards;
	ptGetSupplyChestSuccU->CARD_1_TYPE = cardType[1];
	ptGetSupplyChestSuccU->CARD_1_NUM = numOfCards[1];
	memcpy(&ptGetSupplyChestSuccU->CARD_1_NAME, pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 1) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_2_TYPE = cardType[2];
	ptGetSupplyChestSuccU->CARD_2_NUM = numOfCards[2];
	memcpy(&ptGetSupplyChestSuccU->CARD_2_NAME, pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 2) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_3_TYPE = cardType[3];
	ptGetSupplyChestSuccU->CARD_3_NUM = numOfCards[3];
	memcpy(&ptGetSupplyChestSuccU->CARD_3_NAME, pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 3) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_4_TYPE = cardType[4];
	ptGetSupplyChestSuccU->CARD_4_NUM = numOfCards[4];
	memcpy(&ptGetSupplyChestSuccU->CARD_4_NAME, pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 4) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_5_TYPE = cardType[5];
	ptGetSupplyChestSuccU->CARD_5_NUM = numOfCards[5];
	memcpy(&ptGetSupplyChestSuccU->CARD_5_NAME,pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 5) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_6_TYPE = cardType[6];
	ptGetSupplyChestSuccU->CARD_6_NUM = numOfCards[6];
	memcpy(&ptGetSupplyChestSuccU->CARD_6_NAME, pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 6) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_7_TYPE = cardType[7];
	ptGetSupplyChestSuccU->CARD_7_NUM = numOfCards[7];
	memcpy(&ptGetSupplyChestSuccU->CARD_7_NAME, pointerForNameOfCard, sizeof(char)*32);
	if(kindsOfCards == 7) return;

	pointerForNameOfCard = pointerForNameOfCard + sizeof(char)*32;
	ptGetSupplyChestSuccU->CARD_8_TYPE = cardType[8];
	ptGetSupplyChestSuccU->CARD_8_NUM = numOfCards[8];
	memcpy(&ptGetSupplyChestSuccU->CARD_8_NAME, pointerForNameOfCard, sizeof(char)*32);
	return;
}

// 该函数在更新数据时存在中断风险, 需要下一步优化
BOOL CGameIocp::updateDBwithDataInSupplyChest(S_PT_GET_SUPPLY_CHEST_SUCC_U* ptGetSupplyChestSuccU, int userId, char* password, int kindsOfCards)
{
	CMySqlSession * mySqlSession = mMySqlSessionPool.GetFreeSession();

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = %d and %s = '%s'", USER_TABLE_NAME, "COINS", ptGetSupplyChestSuccU->COINS,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", USER_TABLE_NAME, "DIAMONDS", ptGetSupplyChestSuccU->DIAMONDS,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_1_NAME, ptGetSupplyChestSuccU->CARD_1_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 1) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_2_NAME, ptGetSupplyChestSuccU->CARD_2_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 2) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_3_NAME, ptGetSupplyChestSuccU->CARD_3_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 3) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_4_NAME, ptGetSupplyChestSuccU->CARD_4_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 4) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_5_NAME, ptGetSupplyChestSuccU->CARD_5_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 5) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_6_NAME, ptGetSupplyChestSuccU->CARD_6_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 6) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", EQUIPMENT_TABLE_NAME, ptGetSupplyChestSuccU->CARD_7_NAME, ptGetSupplyChestSuccU->CARD_7_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 7) return TRUE;

	sprintf(mySqlSession->mCommand, "update %s set %s = %d where %s = %d and %s = '%s'", USER_TABLE_NAME, ptGetSupplyChestSuccU->CARD_8_NAME, ptGetSupplyChestSuccU->CARD_8_NUM,"USER_ID", userId, "PASSWORD", password);
	if(!mySqlSession->OPERATE_DATA()) return FALSE;
	if(kindsOfCards == 8) return TRUE;
}




















