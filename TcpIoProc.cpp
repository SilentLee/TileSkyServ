#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "Stream.h"
#include "IniFile.h"

#include "ErrorCode.h"
#include "UserProtocol.h"
#include "User_PT_Structure.h"
#include "User_PT_ReadPacket.h"
#include "User_PT_WritePacket.h"

#include "PurchaseProtocol.h"
#include "PURCHASE_PT_Structure.h"
#include "PURCHASE_PT_ReadPacket.h"
#include "PURCHASE_PT_WritePacket.h"

#include "DoubleBattleProtocol.h"
#include "DOUBLE_BATTLE_PT_Structure.h"
#include "DOUBLE_BATTLE_PT_ReadPacket.h"
#include "DOUBLE_BATTLE_PT_WritePacket.h"

#include "BATTLE_Protocol.h"
#include "BATTLE_PT_ReadPacket.h"
#include "BATTLE_PT_Structure.h"
#include "BATTLE_PT_WritePacket.h"

#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

// DWORD dataLength 为接收数据的长度
VOID CGameIocp::OnIoRead(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	DWORD	Protocol						= 0;
	BYTE	Packet[MAX_BUFFER_LENGTH]		= {0,};
	DWORD	PacketLength					= 0;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (ConnectedUser->ReadPacketForIocp(dataLength))
	{
		while (ConnectedUser->GetPacket(Protocol, Packet, PacketLength))
		{
			switch(Protocol)
			{
			case PT_BATTLE_SEARCH_ROOM:
				onPT_BATTLE_SEARCH_ROOM(ConnectedUser, Packet);
				break;

			case PT_BATTLE_ARRANGE_WEAPON:
				onPT_BATTLE_ARRANGE_WEAPON(ConnectedUser, Packet);
				break;
			//// 用户账户注册 登录 信息更新数据包处理
			//// 1. 游客注册
			//case PT_VISITOR_REGIST:
			//	onPT_VISITOR_REGIST(ConnectedUser, Packet);
			//	break;
			//// 2. 用户名注册
			//case PT_USER_NAME_REGIST:
			//	onPT_USER_NAME_REGIST(ConnectedUser, Packet);
			//	break;
			//// 3. 绑定账户注册
			//case PT_BIND_ACCOUNT_REGIST:
			//	onPT_BIND_ACCOUNT_REGIST(ConnectedUser, Packet);
			//	break;
			//// 4. 手机号码注册
			//case PT_CELLPHONE_NO_REGIST:
			//	onPT_CELLPHONE_NO_REGIST(ConnectedUser, Packet);
			//	break;
			//// 5. 用户名登录
			//case PT_USER_NAME_LOGIN:
			//	onPT_USER_NAME_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 6. 绑定账户登录
			//case PT_BIND_ACCOUNT_LOGIN:
			//	onPT_BIND_ACCOUNT_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 7. 手机号码登录
			//case PT_CELLPHONE_NO_LOGIN:
			//	onPT_CELLPHONE_NO_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 8. 游客默认登录
			//case PT_VISITOR_DEFAULT_LOGIN:
			//	onPT_VISITOR_DEFAULT_LOGIN(ConnectedUser, Packet);
			//	break;
			//// 12. 添加昵称
			//case PT_ADD_NICKNAME:
			//	onPT_ADD_NICKNAME(ConnectedUser, Packet);
			//	break;
			//// 13. 添加绑定账户
			//case PT_ADD_BIND_ACCOUNT:
			//	onPT_ADD_BIND_ACCOUNT(ConnectedUser, Packet);
			//	break;
			//// 14. 添加手机号码
			//case PT_ADD_CELLPHONE_NO:
			//	onPT_ADD_CELLPHONE_NO(ConnectedUser, Packet);
			//	break;
			//// 15. 更改密码
			//case PT_UPDATE_PASSWORD:
			//	onPT_UPDATE_PASSWORD(ConnectedUser, Packet);
			//	break;
			//// 16. 更改昵称
			//case PT_UPDATE_NICKNAME:
			//	onPT_UPDATE_NICKNAME(ConnectedUser, Packet);
			//	break;
			//// 17. 更改手机号码
			//case PT_UPDATE_CELLPHONE_NO:
			//	onPT_UPDATE_CELLPHONE_NO(ConnectedUser, Packet);
			//	break;
			//// 18. 更改用户战绩
			//case PT_UPDATE_RECORD:
			//	onPT_UPDATE_RECORD(ConnectedUser, Packet);
			//	break;
			//	
			//// 用户购买请求数据包处理
			//// 1. 购买金币
			//case PT_BUY_COINS:
			//	onPT_BUY_COINS(ConnectedUser, Packet);
			//	break;
			//// 2. 购买钻石
			//case PT_BUY_DIAMONDS:
			//	onPT_BUY_DIAMONDS(ConnectedUser, Packet);
			//	break;
			//// 3. 购买卡牌
			//case PT_BUY_CARDS:
			//	onPT_BUY_CARDS(ConnectedUser, Packet);
			//	break;

			//case PT_ACQUIRE_CHEST:
			//	onPT_ACQUIRE_CHEST(ConnectedUser, Packet);
			//	break;

			//// 5. 打开补给箱
			//case PT_OPEN_SUPPLY_CHEST:
			//	onPT_OPEN_SUPPLY_CHEST(ConnectedUser, Packet);
			//	break;

			//case PT_LOAD_CARDS_ON_SALE:
			//	onPT_LOAD_CARDS_ON_SALE(ConnectedUser, Packet);
			//	break;

			//case PT_UPGRADE_CARD:
			//	onPT_UPGRADE_CARD(ConnectedUser, Packet);
			//	break;

			//// 寻找对战房间
			//case PT_DOUBLE_BATTLE_SEARCH_ROOM:
			//	onPT_DOUBLE_BATTLE_SEARCH_ROOM(ConnectedUser, Packet);
			//	break;

			//// 接收客户端发送的飞机布设消息
			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_STEALTH_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_STEALTH_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_STEALTH_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_STEALTH_PLANE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_STEALTH_PLANE:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_STEALTH_PLANE(ConnectedUser, Packet);
			//	break;

			//// 接收客户端发来的武器操作消息
			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_ANTI_STEALTH_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_ANTI_STEALTH_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_ANTI_STEALTH_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_ANTI_STEALTH_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_ANTI_STEALTH_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_ANTI_STEALTH_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_MISSILE:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_MISSILE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_MISSILE:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_MISSILE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_MISSILE:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_MISSILE(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_RADAR(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_RADAR:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_RADAR(ConnectedUser, Packet);
			//	break;
			//case PT_DOUBLE_BATTLE_INSERT_SENIOR_CANNONBALL:
			//	onPT_DOUBLE_BATTLE_INSERT_SENIOR_CANNONBALL(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_MIDDLE_CANNONBALL:
			//	onPT_DOUBLE_BATTLE_INSERT_MIDDLE_CANNONBALL(ConnectedUser, Packet);
			//	break;

			//case PT_DOUBLE_BATTLE_INSERT_JUNIOR_CANNONBALL:
			//	onPT_DOUBLE_BATTLE_INSERT_JUNIOR_CANNONBALL(ConnectedUser, Packet);
			//	break;
			}
		}
	}

	// 读取数据后, 恢复等待处理读取数据的状态
	if (!ConnectedUser->InitializeReadForIocp())
		ConnectedUser->End();
	// TODO : 强制结束部分
	// End一定是在之后
	// ConnectedUser->End();
}

// 以下函数: OnIoConnected, OnIoDisconnected, OnIoRead, OnIoWrote
// 由父类 CIocp 中的 WorkerThreadCallback 调用
VOID CGameIocp::OnIoConnected(VOID *object)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	TCHAR	RemoteAddress[32]	= {0,};

	// 获得登录的实体对象的地址
	USHORT	RemotePort			= 0;

	// 获得Accept之后登录的实体对象地址
	ConnectedUser->GetRemoteAddressAfterAccept(RemoteAddress, RemotePort);

	// 将所读取的地址设定到ConnectedUser
	ConnectedUser->SetRealInformation(RemoteAddress, RemotePort);

	// 将客户端的 Socket 连接与完成端口绑定
	if (!CIocp::RegisterSocketToIocp(ConnectedUser->GetSocket(), (ULONG_PTR) ConnectedUser))
	{
		CLog::WriteLog(_T("! OnIoConnected : CIocp::RegisterSocketToIocp"));

		End();

		return;
	}
	// 将对象转换为登录状态
	ConnectedUser->SetIsConnected(TRUE);

	// 运行基本的Iocp获取
	if (!ConnectedUser->InitializeReadForIocp())
	{
		if (!ConnectedUser->Reload(mListenSession.GetSocket()))
		{
			CLog::WriteLog(_T("! OnIoConnected : ConnectedUser->Reload"));

			End();

			return;
		}
	}

	// 增加ConnectedUserManager的当前连接人数
	mConnectedUserManager.IncreaseConnectedUserCount();

	CLog::WriteLog(_T("# New client connected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

VOID CGameIocp::OnIoDisconnected(VOID *object)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	//ConnectedUser->db_UpdateUserAccountInfo(mMySqlSessionPool);
	while(!ConnectedUser->db_UpdateUserAccountInfo(mMySqlSessionPool)) {};

	// 退出用户仍在房间内的情况
	if (ConnectedUser->GetStatus() >= US_ROOM_ENTERING)
	{
		// 通知其他用户并使其退出房间
		CRoom *Room = ConnectedUser->GetEnteredRoom();

		if (Room)
			Room->LeaveRoom(TRUE, this, ConnectedUser);

		// 若当前房间已经没有玩家
		// 结束当前房间内的游戏
		if (!Room->GetCurrentUserCount()) {
			Room->GameEnd(this);
		}
	}

	// 若当前用户连接被 AI 接管, 在用户断开连接时并不立刻重置网络连接, 需等本局游戏结束后再对网络连接进行重置
	// 此处代码待完善?????????????????????????????????????????????????????????????????????????????????
	//if(ConnectedUser->GetStatus() >= AI_GAME_START && ConnectedUser->GetStatus() <= AI_GAME_ENDING) {
	//	return;
	//}

	// 用户断开连接后, 将该 ConnectedUser 重新设置为 Accept 状态, 等待下一个用户连接
	if (!ConnectedUser->Reload(mListenSession.GetSocket()))
	{
		CLog::WriteLog(_T("! OnIoDisconnected : ConnectedUser->Reload"));
		// 若 CConnectedUser 重新载入失败 结束当前 iocp 对象
		End();

		return;
	}

	// 因为断开一个连接, ConnectedUserManager 中当前连接用户数的值减 1
	mConnectedUserManager.DecreaseConnectedUserCount();

	CLog::WriteLog(_T("# Client disconnected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

// OnIoWrite 只需将写完成之后的数据弹出发送数据队列即可
VOID CGameIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;
	// 写入完成时候进行的处理
	ConnectedUser->WriteComplete(); // 在 WriteComplete 函数中, 将发送完成的数据弹出发送数据队列

#ifdef _FULL_DEBUG
	CLog::WriteLog(_T("# Client data wrote : 0x%x(0x%x)(%d)\n"), ConnectedUser, ConnectedUser->GetSocket(), dataLength);
#endif
}

























