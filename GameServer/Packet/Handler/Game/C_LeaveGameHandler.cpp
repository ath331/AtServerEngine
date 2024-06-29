////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_LeaveGameHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "C_LeaveGameHandler.h"
#include "Session/GameSession.h"
#include "Logic/Object/Actor/Player/Player.h"
#include "Logic/Room/Room.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool C_LeaveGameHandler::Handle( PacketSessionPtr& session, Protocol::C_LeaveGame& pkt )
{
	auto gameSession = static_pointer_cast< GameSession >( session );
	if ( !gameSession )
		return false;

	PlayerPtr player = gameSession->player.load();
	if ( !player )
		return false;

	RoomPtr room = player->room.load().lock();
	if ( !room )
		return false;

	room->DoAsync( &Room::HandleLeavePlayer, player );

	return true;
}