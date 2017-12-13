#pragma once

// CRoomManager �̳ж��߳�ͬ������
class CRoomManager : public CMultiThreadSync<CRoomManager>
{
public:
	CRoomManager(VOID);
	virtual ~CRoomManager(VOID);

private:
	DWORD				mMaxRoomCount; // �趨��󷿼���
	DWORD				mCurrentRoomCount; // �趨��ǰ������

	std::vector<CRoom*>	mRoomVectorClassic;	// ���澭�䶷�� CRoom ����ָ���ʸ��ֵ
	std::vector<CRoom*>	mRoomVectorDouble;	// ������������� CRoom ����ָ���ʸ��ֵ
	std::vector<CRoom*>	mRoomVectorTrible;		// ���������Ҷ��� CRoom ����ָ���ʸ��ֵ

public:
	BOOL	Begin(DWORD maxRoomCount); // ��ʼ����
	BOOL	End(VOID); // ��������

	// ˢ�·���״̬�ĺ���
	BOOL	UpdateRooms(CGameIocp *iocp);
	// ��ͻ���ͬ������״̬�ĺ���
	BOOL SyncRooms(CGameIocp *iocp);

	// ʹ�ÿ��ٽ��뷿�书��ʱ�����õĺ���
	CRoom*	QuickJoinRoom(CConnectedUser *connectedUser, ROOM_TYPE roomType);
};
