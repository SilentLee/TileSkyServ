#include "stdafx.h"
#include "UserStruct.h"

S_DOUBLE_BATTLE_ROOM_USER GetDoubleBattleRoomUserPacket
			(S_USER_ACCOUNT_INFO* userAccountInfo, S_USER_EQUIPMENT_INFO* userEquipmentInfo)
{
	S_DOUBLE_BATTLE_ROOM_USER DoubleBattleRoomUser;
	memset(&DoubleBattleRoomUser, 0, sizeof(DoubleBattleRoomUser));

	DoubleBattleRoomUser.SESSION_ID = userAccountInfo->SESSION_ID;
	DoubleBattleRoomUser.USER_ID = userAccountInfo->USER_ID;
	memcpy(DoubleBattleRoomUser.NICKNAME, userAccountInfo->NICKNAME, 32);
	DoubleBattleRoomUser.INTEGRAL_POINTS = userAccountInfo->INTEGRAL_POINTS;
	DoubleBattleRoomUser.LEVEL = userAccountInfo->LEVEL;
	DoubleBattleRoomUser.WIN_TIMES = userAccountInfo->WIN_TIMES;
	DoubleBattleRoomUser.BATTLE_TIMES = userAccountInfo->BATTLE_TIMES;
	DoubleBattleRoomUser.WIN_RATE = userAccountInfo->WIN_RATE;
	DoubleBattleRoomUser.SHOT_DOWN_PLANES = userAccountInfo->SHOT_DOWN_PLANES;
	DoubleBattleRoomUser.SHOT_DOWN_JUNIOR_PLANES = userAccountInfo->SHOT_DOWN_JUNIOR_PLANES;
	DoubleBattleRoomUser.SHOT_DOWN_MIDDLE_PLANES = userAccountInfo->SHOT_DOWN_MIDDLE_PLANES;
	DoubleBattleRoomUser.SHOT_DOWN_SENIOR_PLANES = userAccountInfo->SHOT_DOWN_SENIOR_PLANES;

	DoubleBattleRoomUser.LEVEL_JUNIOR_PLANE = userEquipmentInfo->JUNIOR_PLANE_LEVEL;
	DoubleBattleRoomUser.LEVEL_MIDDLE_PLANE = userEquipmentInfo->MIDDLE_PLANE_LEVEL;
	DoubleBattleRoomUser.LEVEL_SENIOR_PLANE =userEquipmentInfo->SENIOR_PLANE_LEVEL;
	DoubleBattleRoomUser.LEVEL_JUNIOR_STEALTH_PLANE = userEquipmentInfo->JUNIOR_STEALTH_PLANE_LEVEL;
	DoubleBattleRoomUser.LEVEL_MIDDLE_STEALTH_PLANE = userEquipmentInfo->MIDDLE_STEALTH_PLANE_LEVEL;
	DoubleBattleRoomUser.LEVEL_SENIOR_STEALTH_PLANE = userEquipmentInfo->SENIOR_STEALTH_PLANE_LEVEL;
	DoubleBattleRoomUser.LEVEL_SENIOR_ANTI_STEALTH_RADAR = userEquipmentInfo->SENIOR_ANTI_STEALTH_RADAR_LEVEL;
	DoubleBattleRoomUser.LEVEL_MIDDLE_ANTI_STEALTH_RADAR = userEquipmentInfo->MIDDLE_ANTI_STEALTH_RADAR_LEVEL;
	DoubleBattleRoomUser.LEVEL_JUNIOR_ANTI_STEALTH_RADAR = userEquipmentInfo->JUNIOR_ANTI_STEALTH_RADAR_LEVEL;
	DoubleBattleRoomUser.LEVEL_SENIOR_MISSILE = userEquipmentInfo->SENIOR_MISSILE_LEVEL;
	DoubleBattleRoomUser.LEVEL_MIDDLE_MISSILE = userEquipmentInfo->MIDDLE_MISSILE_LEVEL;
	DoubleBattleRoomUser.LEVEL_JUNIOR_MISSILE = userEquipmentInfo->JUNIOR_MISSILE_LEVEL;
	DoubleBattleRoomUser.LEVEL_SENIOR_RADAR = userEquipmentInfo->SENIOR_RADAR_LEVEL;
	DoubleBattleRoomUser.LEVEL_MIDDLE_RADAR = userEquipmentInfo->MIDDLE_RADAR_LEVEL;
	DoubleBattleRoomUser.LEVEL_JUNIOR_RADAR = userEquipmentInfo->JUNIOR_RADAR_LEVEL;
	DoubleBattleRoomUser.LEVEL_SENIOR_CANNONBALL = userEquipmentInfo->SENIOR_CANNONBALL_LEVEL;
	DoubleBattleRoomUser.LEVEL_MIDDLE_CANNONBALL = userEquipmentInfo->MIDDLE_CANNONBALL_LEVEL;
	DoubleBattleRoomUser.LEVEL_JUNIOR_CANNONBALL = userEquipmentInfo->JUNIOR_CANNONBALL_LEVEL;

	return DoubleBattleRoomUser;
}