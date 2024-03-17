#include "pch.h"
#include "ClientPacketHandler.h"
#include "Logic/Actor/Player/Player.h"
#include "Logic/Room/Room.h"
#include "Session/GameSession.h"


PacketHandlerFunc GPacketHandler[ UINT16_MAX ];


// 직접 컨텐츠 작업자


bool Handle_INVALID( PacketSessionRef& session, BYTE* buffer, int32 len )
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>( buffer );
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN( PacketSessionRef& session, Protocol::C_LOGIN& pkt )
{
	//GameSessionPtr gameSession = static_pointer_cast<GameSession>( session );

	//// TODO : Validation 체크

	//Protocol::S_LOGIN loginPkt;
	//loginPkt.set_success( true );

	//// DB에서 플레이 정보를 긁어온다
	//// GameSession에 플레이 정보를 저장 (메모리)

	//// ID 발급 (DB 아이디가 아니고, 인게임 아이디)
	//static Atomic<uint64> idGenerator = 1;

	//{
	//	auto player = loginPkt.add_players();
	//	player->set_name( u8"DB에서긁어온이름1" );
	//	player->set_playertype( Protocol::PLAYER_TYPE_KNIGHT );

	//	PlayerPtr playerPtr = MakeShared<Player>();
	//	playerPtr->playerId = idGenerator++;
	//	playerPtr->name = player->name();
	//	playerPtr->type = player->playertype();
	//	playerPtr->ownerSession = gameSession;

	//	gameSession->_players.push_back( playerPtr );
	//}

	//{
	//	auto player = loginPkt.add_players();
	//	player->set_name( u8"DB에서긁어온이름2" );
	//	player->set_playertype( Protocol::PLAYER_TYPE_MAGE );

	//	PlayerPtr playerPtr = MakeShared<Player>();
	//	playerPtr->playerId = idGenerator++;
	//	playerPtr->name = player->name();
	//	playerPtr->type = player->playertype();
	//	playerPtr->ownerSession = gameSession;

	//	gameSession->_players.push_back( playerPtr );
	//}

	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer( loginPkt );
	//session->Send( sendBuffer );

	return true;
}

//bool Handle_C_ENTER_GAME( PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt )
//{
//	GameSessionPtr gameSession = static_pointer_cast<GameSession>( session );
//
//	uint64 index = pkt.playerindex();
//	// TODO : Validation
//
//	gameSession->_currentPlayer = gameSession->_players[ index ]; // READ_ONLY?
//	gameSession->_room = GRoom;
//
//	GRoom->DoAsync( &Room::Enter, gameSession->_currentPlayer );
//
//	Protocol::S_ENTER_GAME enterGamePkt;
//	enterGamePkt.set_success( true );
//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( enterGamePkt );
//	gameSession->_currentPlayer->ownerSession->Send( sendBuffer );
//
//	return true;
//}

bool Handle_C_CHAT( PacketSessionRef& session, Protocol::C_CHAT& pkt )
{
	std::cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg( pkt.msg() );
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( chatPkt );

	GRoom->DoAsync( &Room::Broadcast, sendBuffer );

	return true;
}
