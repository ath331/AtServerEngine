////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class InfoManagerTemplate
{
public:
	/// 생성자
	InfoManagerTemplate();

	/// 소멸자
	~InfoManagerTemplate();

public:
	/// Id를 반환한다.
	AtInt32 GetId() { return m_id; }

	/// Id를 세팅한다.
	AtVoid SetId( AtInt32 id ) { m_id = id; }

	/// Type을 반환한다.
	Protocol::EActorType GetType() { return m_type; }

	/// Type을 세팅한다.
	AtVoid SetType( Protocol::EActorType type ) { m_type = type; }

private:
	/// Id
	AtInt32 m_id;

	// Type
	Protocol::EActorType m_type;
};