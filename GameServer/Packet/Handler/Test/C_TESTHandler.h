////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_TESTHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_TESTHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////
class C_TESTHandler
{
public:
	// HandlerRun
	static AtBool Handle( PacketSessionRef& session, Protocol::C_TEST& pkt );
};