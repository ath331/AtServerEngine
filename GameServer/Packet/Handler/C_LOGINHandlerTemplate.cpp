////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Server use handler. !!Auto Make File!!
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "ClientPacketHandler.h"
#include "Login/C_LOGINHandler.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handle_C_LOGINTemplate( PacketSessionRef& session, Protocol::C_LOGIN& pkt )
{
	return C_LOGINHandler::Handle( session, pkt );
}