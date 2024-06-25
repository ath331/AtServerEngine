////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room File
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "Room.h"
#include "Logic/Actor/Player/Player.h"
#include "Session/GameSession.h"
#include "Packet/Handler/ClientPacketHandler.h"


RoomPtr GRoom = make_shared< Room >();


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif ������
////////////////////////////////////////////////////////////////////////////////////////////////////
Room::Room()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif �Ҹ���
////////////////////////////////////////////////////////////////////////////////////////////////////
Room::~Room()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif �÷��̾ �濡 �����Ų��.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool Room::HandleEnterPlayer( PlayerPtr player )
{
	AtBool success = EnterPlayer( player );

	// ���� ��ġ
	player->playerInfo->set_x  ( Utils::GetRandom( 0.f, 500.f ) );
	player->playerInfo->set_y  ( Utils::GetRandom( 0.f, 500.f ) );
	//player->playerInfo->set_z  ( Utils::GetRandom( 0.f, 500.f ) );
	player->playerInfo->set_z  ( 100.f );
	player->playerInfo->set_yaw( Utils::GetRandom( 0.f, 100.f ) );

	// ���� ����� ���� �÷��̾�� �˸���
	{
		Protocol::S_EnterGame enterGamePkt;
		enterGamePkt.set_success( success );

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom( *player->playerInfo );
		enterGamePkt.set_allocated_player( playerInfo );
		//enterGamePkt.release_player();

		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( enterGamePkt );
		if ( auto session = player->session.lock() )
			session->Send( sendBuffer );
	}

	// ���� ����� �ٸ� �÷��̾�� �˸���
	{
		Protocol::S_Spawn spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom( *player->playerInfo );

		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( spawnPkt );
		Broadcast( sendBuffer, player->playerInfo->id() );
	}

	// ���� ������ �÷��̾� ����� ���� �÷��̾����� �������ش�
	{
		Protocol::S_Spawn spawnPkt;

		for ( auto& item : _players )
		{
			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom( *item.second->playerInfo );
		}

		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( spawnPkt );
		if ( auto session = player->session.lock() )
			session->Send( sendBuffer );
	}

	return success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif �÷��̾ �濡�� ��������.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool Room::HandleLeavePlayer( PlayerPtr player )
{
	if ( player == nullptr )
		return false;
	
	const uint64 objectId = player->playerInfo->id();
	bool success = LeavePlayer( objectId );
	
	// ���� ����� �����ϴ� �÷��̾�� �˸���
	{
		Protocol::S_LeaveGame leaveGamePkt;
	
		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( leaveGamePkt );
		if ( auto session = player->session.lock() )
			session->Send( sendBuffer );
	}
	
	// ���� ����� �˸���
	{
		Protocol::S_DeSpawn despawnPkt;
		despawnPkt.add_ids( objectId );
	
		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( despawnPkt );
		Broadcast( sendBuffer, objectId );
	
		if ( auto session = player->session.lock() )
			session->Send( sendBuffer );
	}

	return success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif �÷��̾��� �������� ó���Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtVoid Room::HandlePlayerMove( Protocol::C_Move pkt )
{
	const AtInt64 id = pkt.info().id();
	if ( _players.find( id ) == _players.end() )
		return;

	// TODO : ��ġ üũ�ϱ�
	PlayerPtr player = _players[ id ];
	player->playerInfo->CopyFrom( pkt.info() );

	Protocol::S_Move movePkt;
	auto* info =  movePkt.mutable_info();
	info->CopyFrom( pkt.info() );

	SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( movePkt );
	Broadcast( sendBuffer, id );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room��ü�� ��ȯ�Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////////
RoomPtr Room::GetPtr()
{
	return static_pointer_cast<Room>( shared_from_this() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif �÷��̾ �濡 �����Ų��.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool Room::EnterPlayer( PlayerPtr player )
{
	if ( _players.find( player->playerInfo->id() ) != _players.end() )
		return false;

	_players.insert( make_pair( player->playerInfo->id(), player ) );

	player->room.store( GetPtr() );

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif �÷��̾ �濡�� ��������.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool Room::LeavePlayer( uint64 objectId )
{
	if ( _players.find( objectId ) == _players.end() )
		return false;

	PlayerPtr player = _players[ objectId ];
	player->room.store( weak_ptr<Room>() );

	_players.erase( objectId );

	return true;
}

AtVoid Room::Broadcast( SendBufferPtr sendBuffer, uint64 exceptId )
{
	for ( auto& item : _players )
	{
		PlayerPtr player = item.second;
		if ( player->playerInfo->id() == exceptId )
			continue;

		if ( GameSessionPtr session = player->session.lock() )
			session->Send( sendBuffer );
	}
}