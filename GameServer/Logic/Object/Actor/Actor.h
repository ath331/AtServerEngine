////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Actor class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Packet/Protocol.pb.h"
#include "Logic/Object/Object.h"


class GameSession;
class Room;


class Actor
	:
public Object
{
public:
	/// 생성자
	Actor();

	/// 소멸자
	virtual ~Actor();

protected:
	/// 엑터 타입
	Protocol::EActorType m_actorType;
};

// 포인터 타입 정의
USING_SHARED_PTR( Actor );