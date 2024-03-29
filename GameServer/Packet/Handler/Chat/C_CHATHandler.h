////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_CHATHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_CHATHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////
class C_CHATHandler
{
public:
	// HandlerRun
	static AtBool Handle( PacketSessionRef& session, Protocol::C_CHAT& pkt );
};
