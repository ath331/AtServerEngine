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
	/// 생성자
	Room();

	/// 소멸자
	virtual ~Room();

	/// 플레이어를 방에 입장시킨다. ( Thread Safe )
	AtBool HandleEnterPlayerLocked( PlayerPtr player );

	/// 플레이어를 방에서 내보낸다. ( Thread Safe )
	AtBool HandleLeavePlayerLocked( PlayerPtr player );

private:
	/// 플레이어를 방에 입장시킨다.
	AtBool EnterPlayer( PlayerPtr player );

	/// 플레이어를 방에서 내보낸다.
	AtBool LeavePlayer( uint64 objectId );

	USE_LOCK;

private:
	AtVoid Broadcast( SendBufferPtr sendBuffer, uint64 exceptId = 0 );

private:
	unordered_map<uint64, PlayerPtr > _players;
};

extern RoomPtr GRoom;