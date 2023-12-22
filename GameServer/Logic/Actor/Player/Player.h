#pragma once
#include "../../../Enum.pb.h"
#include "Logic/Room/Room.h"
#include "Session/GameSession.h"


class Player
{
public:
	uint64					playerId = 0;
	string					name;
	Protocol::PlayerType	type = Protocol::PLAYER_TYPE_NONE;
	GameSessionPtr			ownerSession; // Cycle
};


using PlayerPtr = shared_ptr< Player >;