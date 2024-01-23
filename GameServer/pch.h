#pragma once

#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"
// #include "Enum.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Ptr
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logic/Actor/Player/PlayerTypes.h"
#include "Logic/Room/RoomTypes.h"
#include "Session/GameSessionTypes.h"