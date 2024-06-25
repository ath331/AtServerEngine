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
	/// 생성자
	Room();

	/// 소멸자
	virtual ~Room();

	/// 플레이어를 방에 입장시킨다.
	AtBool HandleEnterPlayer( PlayerPtr player );

	/// 플레이어를 방에서 내보낸다.
	AtBool HandleLeavePlayer( PlayerPtr player );

	/// 플레이어의 움직임을 처리한다.
	AtVoid HandlePlayerMove( Protocol::C_Move pkt );

	/// Room객체를 반환한다.
	RoomPtr GetPtr();

private:
	/// 플레이어를 방에 입장시킨다.
	AtBool EnterPlayer( PlayerPtr player );

	/// 플레이어를 방에서 내보낸다.
	AtBool LeavePlayer( uint64 objectId );

private:
	AtVoid Broadcast( SendBufferPtr sendBuffer, uint64 exceptId = 0 );

private:
	unordered_map<uint64, PlayerPtr > _players;
};

extern RoomPtr GRoom;