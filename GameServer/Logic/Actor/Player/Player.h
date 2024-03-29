#pragma once
#include "../../../Packet/Enum.pb.h"


class Player
{
public:
	uint64					playerId = 0;
	string					name;
	Protocol::PlayerType	type = Protocol::PLAYER_TYPE_NONE;
	GameSessionPtr			ownerSession; // Cycle
};
