#pragma once
#include "JobQueue.h"
#include "Logic/Actor/Player/PlayerTypes.h"


class Room : public JobQueue
{
public:
	// 싱글쓰레드 환경인마냥 코딩
	void Enter( PlayerPtr player );
	void Leave( PlayerPtr player );
	void Broadcast( SendBufferRef sendBuffer );

private:
	map< uint64, PlayerPtr > _players;
};


extern shared_ptr< Room > GRoom;