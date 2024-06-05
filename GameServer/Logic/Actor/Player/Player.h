////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Player class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Packet/Protocol.pb.h"


class GameSession;
class Room;


class Player
	:
public enable_shared_from_this< Player >
{
public:
	/// 생성자
	Player();

	/// 소멸자
	virtual ~Player();

public:
	/// 세션 정보
	weak_ptr< GameSession > session;

	/// 플레이어 정보
	Protocol::PlayerInfo* playerInfo;

	/// 접속중인 Room 정보
	atomic< weak_ptr< Room >> room;
};
