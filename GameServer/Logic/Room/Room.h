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

	/// 오브젝트를 입장시킨다.
	AtBool EnterRoom( ObjectPtr object, AtBool randPos = true );

	/// 오브젝트를 퇴장시킨다.
	AtBool LeaveRoom( ObjectPtr object );

	/// 플레이어를 방에 입장시킨다.
	AtBool HandleEnterPlayer( PlayerPtr player );

	/// 플레이어를 방에서 내보낸다.
	AtBool HandleLeavePlayer( PlayerPtr player );

	/// 플레이어의 움직임을 처리한다.
	AtVoid HandlePlayerMove( Protocol::C_Move pkt );

public:
	/// 룸을 업데이트한다.
	AtVoid UpdateTick();

	/// Room객체를 반환한다.
	RoomPtr GetPtr();

private:
	/// 오브젝트를 추가한다.
	AtBool AddObject( ObjectPtr object );

	/// 오브젝트를 제거한다.
	AtBool RemoveObject( uint64 objectId );

private:
	AtVoid Broadcast( SendBufferPtr sendBuffer, uint64 exceptId = 0 );

private:
	unordered_map<uint64, ObjectPtr > m_objects;
};

extern RoomPtr GRoom;