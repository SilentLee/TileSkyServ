#pragma once
#include "ConnectedUser.h"
#include "BattleField.h"
#include "GameCtrlParams.h"
//class CConnectedUser;
class CGameIocp;

using namespace std;

typedef enum ROOM_STATUS
{
	ROOM_STATUS_EMPTY = 0,
	ROOM_STATUS_WAITING,
	ROOM_STATUS_READY,
	ROOM_STATUS_GAME_START,
	ROOM_STATUS_GAME_IN_PROGRESS,
	ROOM_STATUS_GAME_ENDING,
	ROOM_STATUS_GAME_ENDED
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
	void SetBattleField(CBattleField* battleField) { mBattleField = battleField; };
	CBattleField* GetBattleField() { return mBattleField; };


	// �û����뷿�� �� �뿪����ʱ���õĺ���
	bool					JoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType);
	bool					LeaveRoom(bool isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser);


	// ���÷������������״̬
	void					SetUserStatusAll(ENUM_USER_STATUS status);
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
	inline ROOM_TYPE GetType(){ CThreadSync Sync; return mType; }

	inline void SetStatus(ROOM_STATUS status){CThreadSync Sync;mStatus = status;};
	inline ROOM_STATUS GetStatus(){ CThreadSync Sync; return mStatus; };

	//////////////////////////////////////////////////////////////////////////
	// ��Ϸ������
	void BroadcastBattleSituation();
	ROOM_STATUS UpdateGameProcess();
	//////////////////////////////////////////////////////////////////////////
};
