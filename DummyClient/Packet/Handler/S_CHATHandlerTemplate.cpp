////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Client use handler. !!Auto Make File!!
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "ServerPacketHandler.h"
#include "Chat/S_CHATHandler.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handle_S_CHATTemplate( PacketSessionRef& session, Protocol::S_CHAT& pkt )
{
	return S_CHATHandler::Handle( session, pkt );
}