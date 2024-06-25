////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_MoveHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "C_MoveHandler.h"
#include "Session/GameSession.h"
#include "Logic/Actor/Player/Player.h"
#include "Logic/Room/Room.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool C_MoveHandler::Handle( PacketSessionPtr& session, Protocol::C_Move& pkt )
{
	auto gameSession = static_pointer_cast<GameSession>( session );
	if ( !gameSession )
		return false;

	PlayerPtr player = gameSession->player.load();
	if ( !player )
		return false;

	RoomPtr room = player->room.load().lock();
	if ( !room )
		return false;

	room->DoAsync( &Room::HandlePlayerMove, pkt );

	return true;
}