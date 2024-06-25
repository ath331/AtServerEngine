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
// @breif 생성자
////////////////////////////////////////////////////////////////////////////////////////////////////
Room::Room()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif 소멸자
////////////////////////////////////////////////////////////////////////////////////////////////////
Room::~Room()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif 플레이어를 방에 입장시킨다.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool Room::HandleEnterPlayer( PlayerPtr player )
{
	AtBool success = EnterPlayer( player );

	// 랜덤 위치
	player->playerInfo->set_x  ( Utils::GetRandom( 0.f, 500.f ) );
	player->playerInfo->set_y  ( Utils::GetRandom( 0.f, 500.f ) );
	//player->playerInfo->set_z  ( Utils::GetRandom( 0.f, 500.f ) );
	player->playerInfo->set_z  ( 100.f );
	player->playerInfo->set_yaw( Utils::GetRandom( 0.f, 100.f ) );

	// 입장 사실을 신입 플레이어에게 알린다
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

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_Spawn spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom( *player->playerInfo );

		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( spawnPkt );
		Broadcast( sendBuffer, player->playerInfo->id() );
	}

	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
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
// @breif 플레이어를 방에서 내보낸다.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool Room::HandleLeavePlayer( PlayerPtr player )
{
	if ( player == nullptr )
		return false;
	
	const uint64 objectId = player->playerInfo->id();
	bool success = LeavePlayer( objectId );
	
	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	{
		Protocol::S_LeaveGame leaveGamePkt;
	
		SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( leaveGamePkt );
		if ( auto session = player->session.lock() )
			session->Send( sendBuffer );
	}
	
	// 퇴장 사실을 알린다
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
// @breif 플레이어의 움직임을 처리한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtVoid Room::HandlePlayerMove( Protocol::C_Move pkt )
{
	const AtInt64 id = pkt.info().id();
	if ( _players.find( id ) == _players.end() )
		return;

	// TODO : 위치 체크하기
	PlayerPtr player = _players[ id ];
	player->playerInfo->CopyFrom( pkt.info() );

	Protocol::S_Move movePkt;
	auto* info =  movePkt.mutable_info();
	info->CopyFrom( pkt.info() );

	SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( movePkt );
	Broadcast( sendBuffer, id );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Room객체를 반환한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
RoomPtr Room::GetPtr()
{
	return static_pointer_cast<Room>( shared_from_this() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif 플레이어를 방에 입장시킨다.
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
// @breif 플레이어를 방에서 내보낸다.
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