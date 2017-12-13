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

#include "BattleField.h"

CRoomManager::CRoomManager(VOID)
{
}

CRoomManager::~CRoomManager(VOID)
{
}

// 开始对象函数
// 通过参数获得最大房间数
// 该房间的最大数设置与最大人数相同
BOOL CRoomManager::Begin(DWORD maxRoomCount)
{
	if (maxRoomCount <= 0)
		return FALSE;

	mMaxRoomCount = maxRoomCount;

	// 创建房间对象, 在可以进行管理的指针矢量中添加值
	for (DWORD i=0;i<mMaxRoomCount;i++)
	{
		CRoom *RoomClassic = new CRoom();
		CRoom *RoomDouble = new CRoom();
		CRoom *RoomTrible = new CRoom();

		// 在这里设置经典斗场房间编号为 i
		if (RoomClassic->Begin(i))
			mRoomVectorClassic.push_back(RoomClassic);
		else
		{
			// 在失败的情况下立即结束
			End();
			return FALSE;
		}

		// 在这里设置二人死斗房间编号为 i
		if (RoomDouble->Begin(i))
			mRoomVectorDouble.push_back(RoomDouble);
		else
		{
			// 在失败的情况下立即结束
			End();
			return FALSE;
		}

		// 在这里设置三人乱斗房间编号为 i
		if (RoomTrible->Begin(i))
			mRoomVectorTrible.push_back(RoomTrible);
		else
		{
			// 在失败的情况下立即结束
			End();
			return FALSE;
		}
	}

	return TRUE;
}

// 结束对象函数
BOOL CRoomManager::End(VOID)
{
	CThreadSync Sync;

	for (DWORD i=0;i<mRoomVectorClassic.size();i++)
	{
		CRoom *Room = mRoomVectorClassic[i];
		Room->End();
		delete Room;
	}
	mRoomVectorClassic.clear();

	for (DWORD i=0;i<mRoomVectorDouble.size();i++)
	{
		CRoom *Room = mRoomVectorDouble[i];
		Room->End();
		delete Room;
	}
	mRoomVectorDouble.clear();

	for (DWORD i=0;i<mRoomVectorTrible.size();i++)
	{
		CRoom *Room = mRoomVectorTrible[i];
		Room->End();
		delete Room;
	}
	mRoomVectorTrible.clear();

	return TRUE;
}

// 检索玩家可以进入的房间
// 优先选择已有玩家在其中等待的房间
// 次优先选择空房间
// 如上述两种房间均不存在 检索失败 返回房间地址 NULL
CRoom* CRoomManager::QuickJoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType)
{
	CThreadSync Sync;

	if (!connectedUser)
		return NULL;

	CRoom* Room = NULL;

	// 遍历房间动态数组
	for(int i = 0; i < mRoomVectorDouble.size(); i++) {
		// 若第 i 个房间的房间状态为 RM_WAITING 说明此房间有空位 玩家可以进入
		if (mRoomVectorDouble[i]->GetStatus() == ROOM_STATUS_WAITING)
		{
			// 若玩家成功进入房间
			if (mRoomVectorDouble[i]->JoinRoom(connectedUser, roomType)) {
				// 将房间地址复制到 Room 中
				Room = mRoomVectorDouble[i];
				// 跳出遍历循环
				break;
			}
			// 若玩家进入房间失败 继续检索
			continue;
		}
		else if (mRoomVectorDouble[i]->GetStatus() == ROOM_STATUS_EMPTY) {
			// 若检索到状态为空的房间 记录下房间地址 暂不进入房间
			Room = mRoomVectorDouble[i];
		}
	}

	// 若遍历房间数组后未发现有状态为等待的房间
	// 如果有空房间 说明玩家此前没有找到房间进入
	// 进入当前存储于 Room 中的空房间
	// 如果没有空房间 Room 为 NULL 查找房间失败
	if (Room != NULL && Room->GetStatus() == ROOM_STATUS_EMPTY) {
		Room->JoinRoom(connectedUser, roomType);
	}

	return Room;
}

// 每秒都要被调用来处理游戏规则
BOOL CRoomManager::UpdateRooms(CGameIocp *iocp)
{
	CThreadSync Sync;

	// 重复所有房间
	for (int i = 0; i < mRoomVectorDouble.size(); i++)
	{
		CRoom *Room = mRoomVectorDouble[i];

		// 房间正在进行对战时的处理
		if (Room->GetStatus() == ROOM_STATUS_GAME_IN_PROGRESS) {

			// 刷新房间游戏进程
			// 同时读取房间当前状态 检测游戏是否结束
			ROOM_STATUS RoomStatus = Room->UpdateGameProcess();

			// 若游戏结束 执行 Room->GameEnd()
			if (RoomStatus == ROOM_STATUS_GAME_ENDING) {
				Room->GameEnd(iocp);
			}
		}
	}

	return TRUE;
}

// 与客户端同步房间状态的函数
BOOL CRoomManager::SyncRooms(CGameIocp* iocp)
{
	CThreadSync Sync;

	// 遍历所有房间
	for (int i = 0; i < mRoomVectorDouble.size(); i++)
	{
		CRoom* Room = mRoomVectorDouble[i];

		// 房间正在进行对战时的处理
		if (Room->GetStatus() == ROOM_STATUS_GAME_IN_PROGRESS) {
			ROOM_STATUS RoomStatus = Room->SyncGameProcess();
		}
	}

	return true;
}
