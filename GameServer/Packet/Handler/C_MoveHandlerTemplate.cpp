////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Server use handler. !!Auto Make File!!
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "ClientPacketHandler.h"
#include "Actor/C_MoveHandler.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handle_C_MoveTemplate( PacketSessionPtr& session, Protocol::C_Move& pkt )
{
	return C_MoveHandler::Handle( session, pkt );
}