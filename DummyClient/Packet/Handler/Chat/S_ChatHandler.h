////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif S_CHATHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif S_CHATHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////
class S_CHATHandler
{
public:
	// HandlerRun
	static AtBool Handle( PacketSessionRef& session, Protocol::S_CHAT& pkt );
};
