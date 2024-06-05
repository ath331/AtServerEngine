////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif C_LoginHandler class
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "C_LoginHandler.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif HandlerRun
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool C_LoginHandler::Handle( PacketSessionPtr& session, Protocol::C_Login& pkt )
{
	// TODO : DB에서 정보 읽을 타이밍?
	Protocol::S_Login result;

	for ( AtInt32 i = 0; i < 3; i++ )
	{
		auto player = result.add_players();
		player->set_x  ( Utils::GetRandom( 0.f, 100.f ) );
		player->set_y  ( Utils::GetRandom( 0.f, 100.f ) );
		player->set_z  ( Utils::GetRandom( 0.f, 100.f ) );
		player->set_yaw( Utils::GetRandom( 0.f, 100.f ) );
	}

	result.set_success( true );
	SEND_PACKET( session, result );

	return true;
}