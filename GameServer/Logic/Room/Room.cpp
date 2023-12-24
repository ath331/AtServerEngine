#include "pch.h"
#include "Room.h"


shared_ptr< Room > GRoom = make_shared< Room >();


void Room::Enter( PlayerPtr player )
{
	_players[ player->playerId ] = player;
}

void Room::Leave( PlayerPtr player )
{
	_players.erase( player->playerId );
}

void Room::Broadcast(SendBufferRef sendBuffer)
{
	for ( auto& p : _players )
	{
		p.second->ownerSession->Send( sendBuffer );
	}
}