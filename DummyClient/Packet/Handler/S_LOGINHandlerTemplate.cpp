////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Client use handler. !!Auto Make File!!
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "ServerPacketHandler.h"
#include "Login/S_LOGINHandler.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handle_S_LOGINTemplate( PacketSessionPtr& session, Protocol::S_LOGIN& pkt )
{
	return S_LOGINHandler::Handle( session, pkt );
}