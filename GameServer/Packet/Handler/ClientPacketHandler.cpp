#include "pch.h"
#include "ClientPacketHandler.h"
#include "Logic/Actor/Player/Player.h"
#include "Logic/Room/Room.h"
#include "Session/GameSession.h"


PacketHandlerFunc GPacketHandler[ UINT16_MAX ];


bool Handle_INVALID( PacketSessionRef& session, BYTE* buffer, int32 len )
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>( buffer );
	// TODO : Log
	return false;
}

//bool Handle_C_ENTER_GAME( PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt )
//{
//	GameSessionPtr gameSession = static_pointer_cast<GameSession>( session );
//
//	uint64 index = pkt.playerindex();
//	// TODO : Validation
//
//	gameSession->_currentPlayer = gameSession->_players[ index ]; // READ_ONLY?
//	gameSession->_room = GRoom;
//
//	GRoom->DoAsync( &Room::Enter, gameSession->_currentPlayer );
//
//	Protocol::S_ENTER_GAME enterGamePkt;
//	enterGamePkt.set_success( true );
//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( enterGamePkt );
//	gameSession->_currentPlayer->ownerSession->Send( sendBuffer );
//
//	return true;
//}