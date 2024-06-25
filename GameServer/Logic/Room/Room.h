////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room File
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "JobQueue.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room class
////////////////////////////////////////////////////////////////////////////////////////////////////
class Room 
	: 
	public JobQueue

{
public:
	/// ������
	Room();

	/// �Ҹ���
	virtual ~Room();

	/// �÷��̾ �濡 �����Ų��.
	AtBool HandleEnterPlayer( PlayerPtr player );

	/// �÷��̾ �濡�� ��������.
	AtBool HandleLeavePlayer( PlayerPtr player );

	/// �÷��̾��� �������� ó���Ѵ�.
	AtVoid HandlePlayerMove( Protocol::C_Move pkt );

	/// Room��ü�� ��ȯ�Ѵ�.
	RoomPtr GetPtr();

private:
	/// �÷��̾ �濡 �����Ų��.
	AtBool EnterPlayer( PlayerPtr player );

	/// �÷��̾ �濡�� ��������.
	AtBool LeavePlayer( uint64 objectId );

private:
	AtVoid Broadcast( SendBufferPtr sendBuffer, uint64 exceptId = 0 );

private:
	unordered_map<uint64, PlayerPtr > _players;
};

extern RoomPtr GRoom;