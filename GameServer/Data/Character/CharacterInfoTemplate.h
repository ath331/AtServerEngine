////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief CharacterInfoTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief CharacterInfoTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class CharacterInfoTemplate
{
public:
	/// Constructor
	CharacterInfoTemplate();

	/// Constructor
	CharacterInfoTemplate(
	    AtInt32 id,
	    AtBool isNonUse,
	    Protocol::EBagType bagType );

	/// Destructor
	~CharacterInfoTemplate();

public:
	/// GetId.
	AtInt32 GetId() { return m_id; }

	/// GetIsNonUse.
	AtBool GetIsNonUse() { return m_isNonUse; }

	/// GetBagType.
	Protocol::EBagType GetBagType() { return m_bagType; }

	/// SetId.
	AtVoid SetId( AtInt32 id ) { m_id = id; }

	/// SetIsNonUse.
	AtVoid SetIsNonUse( AtBool isNonUse ) { m_isNonUse = isNonUse; }

	/// SetBagType.
	AtVoid SetBagType( Protocol::EBagType bagType ) { m_bagType = bagType; }


protected:
	/// id
	AtInt32 m_id;

	/// isNonUse
	AtBool m_isNonUse;

	/// bagType
	Protocol::EBagType m_bagType;
};