#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "Logic/Room/Room.h"


void GameSession::OnConnected()
{
	GSessionManager.Add( static_pointer_cast< GameSession >( shared_from_this() ) );

	cout << "立加" << endl;
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove( static_pointer_cast< GameSession >( shared_from_this() ) );

	if ( _currentPlayer )
	{
		if ( auto room = _room.lock() )
			room->DoAsync( &Room::Leave, _currentPlayer );
	}

	_currentPlayer = nullptr;
	_players.clear();
}

void GameSession::OnRecvPacket( BYTE* buffer, int32 len )
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast< PacketHeader* >( buffer );

	// TODO : packetId 措开 眉农
	ClientPacketHandler::HandlePacket( session, buffer, len );
}

void GameSession::OnSend(int32 len)
{
}