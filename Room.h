#pragma once
#include "ConnectedUser.h"
#include "BattleField.h"
#include "GameCtrlParams.h"
//class CConnectedUser;
class CGameIocp;

using namespace std;

typedef enum ROOM_STATUS
{
	RM_EMPTY = 0,
	RM_WAITING,
	RM_READY,
	RM_GAME_START,
	RM_GAME_IN_PROGRESS,
	RM_GAME_ENDING,
	RM_GAME_ENDED
};

typedef enum ROOM_TYPE
{
	RM_NO_TYPE = 0,
	RM_TYPE_1V1,
	RM_TYPE_2V2
};

class CRoom : public CMultiThreadSync<CRoom>
{
public:
	CRoom(void);
	virtual ~CRoom(void);

private:
	ROOM_TYPE							mType;
	DWORD										mIndex; // ������

	ROOM_STATUS						mStatus;
	bool											mIsGameStarted; // ȷ���Ƿ�ʼ��Ϸ�ı���
	
	//list<CConnectedUser*> mUserList;
	list<CConnectedUser*> mUsersInBlueTeam;
	list<CConnectedUser*> mUsersInRedTeam;

	// ս��
	CBattleField* mBattleField;

	//////////////////////////////////////////////////////////////////////////
	// ��Ϸ��ر���


	//////////////////////////////////////////////////////////////////////////

private:
	// ����Ļ�ϴ�ӡ����Э��
	void printProtocol(int protocol);

public:
	bool					Begin(DWORD index);
	bool					End(void);

	// ս�� BattleField �Ĵ�ȡ����
	void setBattleField(CBattleField* battleField) { mBattleField = battleField; };
	CBattleField* getBattleField() { return mBattleField; };


	// �û����뷿�� �� �뿪����ʱ���õĺ���
	// 1v1
	bool					JoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType);
	bool					LeaveRoom(bool isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser);


	// ���÷������������״̬
	void					SetUserStatusAll(USER_STATUS status);
	// �򷿼��������û��������ݰ�ʱʹ�õĺ���
	bool					WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);

	// �������¿�ʼ��Ϸ��ťʱ���еĺ���
	bool					GameStart(void);
	bool					GameEnd(CGameIocp *iocp);

	// �����Ϸ�����Ƿ�����Ҵ���
	inline bool				GetIsEmpty(void){CThreadSync Sync;return (mUsersInBlueTeam.size() + mUsersInRedTeam.size()) ? false : true;}

	// �����Ϸ�����Ƿ��������
	inline bool				GetIsFull(int maxUserNum){CThreadSync Sync;return (mUsersInBlueTeam.size() + mUsersInRedTeam.size()) == maxUserNum ? true : false;}

	// �����Ϸ���䵱ʱ�Ƿ��Ѿ���ʼ��Ϸ
	inline bool				GetIsGameStarted(void){CThreadSync Synd;return mIsGameStarted;}

	// ��ȡ��ǰ������
	inline DWORD			GetIndex(void){CThreadSync Sync;return mIndex;}

	// ��ȡ��ǰ���������û���
	inline USHORT			GetCurrentUserCount(void){CThreadSync Sync;return (mUsersInBlueTeam.size() + mUsersInRedTeam.size());}

	inline void SetType(ROOM_TYPE type){CThreadSync Sync;mType = type;}
	inline INT getType(){CThreadSync Sync;return mType;}

	inline void SetStatus(ROOM_STATUS status){CThreadSync Sync;mStatus = status;};
	inline INT GetStatus(){CThreadSync Sync;return mStatus;};

	//////////////////////////////////////////////////////////////////////////
	// ��Ϸ������
	void BroadcastBattleSituation(CGameIocp *iocp);
	//////////////////////////////////////////////////////////////////////////
};
