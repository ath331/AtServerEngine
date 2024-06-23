////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_EnterGameHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "C_EnterGameHandler.h"
#include "Logic/Utils/ObjectUtils.h"
#include "Logic/Room/Room.h"
#include "Session/GameSession.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool C_EnterGameHandler::Handle( PacketSessionPtr& session, Protocol::C_EnterGame& pkt )
{
	PlayerPtr player = ObjectUtils::CreatePlayer( static_pointer_cast< GameSession >( session ) );

	GRoom->HandleEnterPlayerLocked( player );

	return true;
}