////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief GameServer.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "Session/GameSession.h"
#include "Session/GameSessionManager.h"
#include "BufferWriter.h"
#include <tchar.h>
#include "Job.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "XmlParser.h"
#include "DBSynchronizer.h"


#include "DB/GenProcedures.h"


#include "Packet/Handler/ClientPacketHandler.h"
#include "Packet/Protocol.pb.h"


/// 프로세스 틱 이넘
enum
{
	WORKER_TICK = 64
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief 쓰레드가 동작하는 함수
////////////////////////////////////////////////////////////////////////////////////////////////////
void DoWorkerJob( ServerServicePtr& service )
{
	while ( true )
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch( 10 );

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief ServerMain 함수
////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	ASSERT_CRASH( GDBConnectionPool->Connect( 1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\ProjectModels;Database=AtServer;Trusted_Connection=Yes;" ) );

	DBConnection* dbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync( *dbConn );
	dbSync.Synchronize( L"DB/GameDB.xml" );

	ClientPacketHandler::Init();

	ServerServicePtr service = MakeShared< ServerService >(
		NetAddress( L"192.168.25.22", 7777 ),
		MakeShared< IocpCore >(),
		MakeShared< GameSession >, // TODO : SessionManager 등
		100 );

	ASSERT_CRASH( service->Start() );

	int32 threadCount = 6;
	for ( int32 i = 0; i < threadCount - 1; i++ )
	{
		GThreadManager->Launch(
			[ &service ]()
			{
				DoWorkerJob( service );
			} );
	}

	// Main Thread
	// DoWorkerJob( service );

	 while ( true )
	 {
	 	Protocol::S_Chat pktChat;
	 	pktChat.set_msg( "HelloWorld" );
	 	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( pktChat );
	 
	 	GSessionManager.Broadcast( sendBuffer );
	 	this_thread::sleep_for( 3s );
	 }

	GThreadManager->Join();
}