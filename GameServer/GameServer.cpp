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
#include "Logic/Room/Room.h"
#include "DB/GenProcedures.h"
#include "Packet/Handler/ClientPacketHandler.h"
#include "Packet/Protocol.pb.h"
#include "Data/InfoManagers.h"


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
AtVoid main()
{
	if ( !Environment::Load( "GameServer.ini" ) )
	{
		WARNNING_LOG( "Failed to load config.ini" );
		return;
	}

	ASSERT_CRASH( InitializeInfoManager() );

	// SqlServer
	//ASSERT_CRASH( GDBConnectionPool->Connect( 1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\ProjectModels;Database=AtServer;Trusted_Connection=Yes;" ) );

	// MySql
	AtString connect = std::format( "Driver={{MySQL ODBC 8.2 UNICODE Driver}};Server={};Port={};Database={};User={};Password={};",
									Environment::Get( "DB_IP"   ),
									Environment::Get( "DB_PORT" ),
									Environment::Get( "DB_NAME" ),
									Environment::Get( "DB_USER" ),
									Environment::Get( "DB_PW"   ) );

	ASSERT_CRASH( GDBConnectionPool->Connect( 1, StringUtils::GetWString( connect ).c_str() ) );

	DBConnection* dbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync( *dbConn );
	dbSync.Synchronize( StringUtils::GetWString( Environment::Get( "DB_ASSET_PATH" ) ).c_str() );


	ClientPacketHandler::Init();

	AtString ip   = Environment::Get( "IP"   );
	AtString port = Environment::Get( "PORT" );

	ServerServicePtr service = MakeShared< ServerService >(
		NetAddress( StringUtils::GetWString( ip ), StringUtils::GetAtInt64( port ) ),
		MakeShared< IocpCore >(),
		MakeShared< GameSession >, // TODO : SessionManager 등
		100 );

	ASSERT_CRASH( service->Start() );

	INFO_LOG_GREEN( "Server Start." );

	int32 threadCount = 6;
	for ( int32 i = 0; i < threadCount - 1; i++ )
	{
		GThreadManager->Launch(
			[ &service ]()
			{
				DoWorkerJob( service );
			} );
	}

	GRoom->DoAsync( &Room::UpdateTick );

	// Main Thread
	// DoWorkerJob( service );

	// while ( true )
	// {
	// 	Protocol::S_Chat pktChat;
	// 	pktChat.set_msg( "HelloWorld" );
	// 	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( pktChat );
	// 
	// 	GSessionManager.Broadcast( sendBuffer );
	// 	this_thread::sleep_for( 3s );
	// }

	// Protocol::S_Login pkt;
	// pkt.set_success( true );
	// 
	// auto sendBuffer = ClientPacketHandler::MakeSendBuffer( pkt );

	GThreadManager->Join();
}