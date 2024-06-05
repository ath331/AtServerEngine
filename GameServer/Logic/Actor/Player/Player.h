////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Player class
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Packet/Protocol.pb.h"


class GameSession;
class Room;


class Player
	:
public enable_shared_from_this< Player >
{
public:
	/// ������
	Player();

	/// �Ҹ���
	virtual ~Player();

public:
	/// ���� ����
	weak_ptr< GameSession > session;

	/// �÷��̾� ����
	Protocol::PlayerInfo* playerInfo;

	/// �������� Room ����
	atomic< weak_ptr< Room >> room;
};
