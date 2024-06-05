////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief ObjectUtils File
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "ObjectUtils.h"
#include "Logic/Actor/Player/Player.h"
#include "Session/GameSession.h"


atomic<int64> ObjectUtils::s_idGenerator = 1;


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief 캐릭터를 생성해서 메모리에 들고 있는다
////////////////////////////////////////////////////////////////////////////////////////////////////
PlayerPtr ObjectUtils::CreatePlayer( GameSessionPtr session )
{
	// ID 생성기
	const int64 newId = s_idGenerator.fetch_add( 1 );

	PlayerPtr player = make_shared<Player>();
	player->playerInfo->set_id( newId );

	player->session = session;
	session->player.store( player );

	return player;
}