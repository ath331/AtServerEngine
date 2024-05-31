#pragma once
#include "Packet/Protocol.pb.h"


#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "AtClient.h"
#endif


using PacketHandlerFunc = std::function<bool(PacketSessionPtr&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


enum : uint16
{
	PKT_C_Login = 1000,
	PKT_S_Login = 1001,
	PKT_C_EnterGame = 1002,
	PKT_S_EnterGame = 1003,
	PKT_C_LeaveGame = 1004,
	PKT_S_LeaveGame = 1005,
	PKT_S_Spawn = 1006,
	PKT_S_DeSpawn = 1007,
	PKT_C_Chat = 1008,
	PKT_S_Chat = 1009,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len);
bool Handle_S_LoginTemplate(PacketSessionPtr& session, Protocol::S_Login& pkt);
bool Handle_S_EnterGameTemplate(PacketSessionPtr& session, Protocol::S_EnterGame& pkt);
bool Handle_S_LeaveGameTemplate(PacketSessionPtr& session, Protocol::S_LeaveGame& pkt);
bool Handle_S_SpawnTemplate(PacketSessionPtr& session, Protocol::S_Spawn& pkt);
bool Handle_S_DeSpawnTemplate(PacketSessionPtr& session, Protocol::S_DeSpawn& pkt);
bool Handle_S_ChatTemplate(PacketSessionPtr& session, Protocol::S_Chat& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_Login] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_Login>(Handle_S_LoginTemplate, session, buffer, len); };
		GPacketHandler[PKT_S_EnterGame] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EnterGame>(Handle_S_EnterGameTemplate, session, buffer, len); };
		GPacketHandler[PKT_S_LeaveGame] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LeaveGame>(Handle_S_LeaveGameTemplate, session, buffer, len); };
		GPacketHandler[PKT_S_Spawn] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_Spawn>(Handle_S_SpawnTemplate, session, buffer, len); };
		GPacketHandler[PKT_S_DeSpawn] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DeSpawn>(Handle_S_DeSpawnTemplate, session, buffer, len); };
		GPacketHandler[PKT_S_Chat] = [](PacketSessionPtr& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_Chat>(Handle_S_ChatTemplate, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferPtr MakeSendBuffer(Protocol::C_Login& pkt) { return MakeSendBuffer(pkt, PKT_C_Login); }
	static SendBufferPtr MakeSendBuffer(Protocol::C_EnterGame& pkt) { return MakeSendBuffer(pkt, PKT_C_EnterGame); }
	static SendBufferPtr MakeSendBuffer(Protocol::C_LeaveGame& pkt) { return MakeSendBuffer(pkt, PKT_C_LeaveGame); }
	static SendBufferPtr MakeSendBuffer(Protocol::C_Chat& pkt) { return MakeSendBuffer(pkt, PKT_C_Chat); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionPtr& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferPtr MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		//SendBufferPtr sendBuffer = GSendBufferManager->Open( packetSize );

	#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferPtr sendBuffer = MakeShared< SendBuffer >( packetSize );
	#else
		SendBufferPtr sendBuffer = make_shared< SendBuffer >( packetSize );
	#endif

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		//ASSERT_CRASH( pkt.SerializeToArray( &header[ 1 ], dataSize ) );
		pkt.SerializeToArray( &header[ 1 ], dataSize );
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};