#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "Packet/Handler/ClientPacketHandler.h"
#include "Utils/Log/AtLog.h"
#include "Utils/String/StringUtils.h"


void GameSession::OnConnected()
{
	GSessionManager.Add( static_pointer_cast< GameSession >( shared_from_this() ) );

	INFO_LOG( StringUtils::ConvertToString( GetAddress().GetIpAddress() ) + " Connected." );
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove( static_pointer_cast< GameSession >( shared_from_this() ) );

	INFO_LOG( StringUtils::ConvertToString( GetAddress().GetIpAddress() ) + " Disconnected." );
}

void GameSession::OnRecvPacket( BYTE* buffer, int32 len )
{
	PacketSessionPtr session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast< PacketHeader* >( buffer );

	// TODO : packetId 대역 체크
	ClientPacketHandler::HandlePacket( session, buffer, len );
}

void GameSession::OnSend(int32 len)
{
}