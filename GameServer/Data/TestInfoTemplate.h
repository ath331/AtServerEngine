////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief TestInfoTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief TestInfoTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class TestInfoTemplate
{
public:
	/// Constructor
	TestInfoTemplate();

	/// Constructor
	TestInfoTemplate(
	    AtInt32 id,
	    Protocol::EActorType type,
	    AtString string );

	/// Destructor
	~TestInfoTemplate();

public:
	/// GetId.
	AtInt32 GetId() { return m_id; }

	/// GetType.
	Protocol::EActorType GetType() { return m_type; }

	/// GetString.
	AtString GetString() { return m_string; }

	/// SetId.
	AtVoid SetId( AtInt32 id ) { m_id = id; }

	/// SetType.
	AtVoid SetType( Protocol::EActorType type ) { m_type = type; }

	/// SetString.
	AtVoid SetString( AtString string ) { m_string = string; }


protected:
	/// id
	AtInt32 m_id;

	/// type
	Protocol::EActorType m_type;

	/// string
	AtString m_string;
};