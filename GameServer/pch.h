#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"
#include "Packet/Handler/ClientPacketHandler.h"
#include "Logic/Utils/Utils.h"


//#include "Packet/Enum.pb.h"
#include "Packet/Protocol.pb.h"
//#include "Packet/Struct.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Ptr
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logic/Actor/Player/PlayerTypes.h"
#include "Logic/Room/RoomTypes.h"
#include "Session/GameSessionTypes.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Packet
////////////////////////////////////////////////////////////////////////////////////////////////////
#define SEND_PACKET( session, pkt )  \
	SendBufferPtr sendBuffer = ClientPacketHandler::MakeSendBuffer( pkt ); \
	session->Send( sendBuffer );



/// Ptr 선언들보다 아래 있어야함? GameSessionClass에서 에러남
#include "Session/GameSession.h"
