////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Object class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Packet/Protocol.pb.h"


class GameSession;
class Room;


class Object
	:
public enable_shared_from_this< Object >
{
public:
	/// 생성자
	Object();

	/// 소멸자
	virtual ~Object();

	/// 플레이어 타입인지 확인한다.
	AtBool IsPlayer() { return m_isPlayer; }

public:
	/// 오브젝트 정보
	Protocol::ObjectInfo* objectInfo;

	// 위치 정보
	Protocol::PosInfo* posInfo;

	/// 접속중인 Room 정보
	atomic< weak_ptr< Room >> room;

protected:
	AtBool m_isPlayer = false;
};
