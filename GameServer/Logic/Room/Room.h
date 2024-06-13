////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room File
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room class
////////////////////////////////////////////////////////////////////////////////////////////////////
class Room : public enable_shared_from_this<Room>
{
public:
	/// ������
	Room();

	/// �Ҹ���
	virtual ~Room();

	/// �÷��̾ �濡 �����Ų��. ( Thread Safe )
	AtBool HandleEnterPlayerLocked( PlayerPtr player );

	/// �÷��̾ �濡�� ��������. ( Thread Safe )
	AtBool HandleLeavePlayerLocked( PlayerPtr player );

	/// �÷��̾��� �������� ó���Ѵ�. ( Thread Safe )
	AtVoid HandlePlayerMoveLocked( Protocol::C_Move& pkt );

private:
	/// �÷��̾ �濡 �����Ų��.
	AtBool EnterPlayer( PlayerPtr player );

	/// �÷��̾ �濡�� ��������.
	AtBool LeavePlayer( uint64 objectId );

	USE_LOCK;

private:
	AtVoid Broadcast( SendBufferPtr sendBuffer, uint64 exceptId = 0 );

private:
	unordered_map<uint64, PlayerPtr > _players;
};

extern RoomPtr GRoom;