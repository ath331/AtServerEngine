#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "Packet/Handler/ClientPacketHandler.h"


void GameSession::OnConnected()
{
	GSessionManager.Add( static_pointer_cast< GameSession >( shared_from_this() ) );

	cout << "立加" << endl;
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove( static_pointer_cast< GameSession >( shared_from_this() ) );
}

void GameSession::OnRecvPacket( BYTE* buffer, int32 len )
{
	PacketSessionPtr session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast< PacketHeader* >( buffer );

	// TODO : packetId 措开 眉农
	ClientPacketHandler::HandlePacket( session, buffer, len );
}

void GameSession::OnSend(int32 len)
{
}