////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif S_SpawnHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif S_SpawnHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////
class S_SpawnHandler
{
public:
	// HandlerRun
	static AtBool Handle( PacketSessionPtr& session, Protocol::S_Spawn& pkt );
};