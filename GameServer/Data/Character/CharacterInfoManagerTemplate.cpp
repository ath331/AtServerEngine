////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief CharacterInfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include "CharacterInfoManagerTemplate.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief GetInfo
////////////////////////////////////////////////////////////////////////////////////////////////////
const CharacterInfo* CharacterInfoManagerTemplate::GetInfo( AtInt32 id )
{
	auto iter = m_infoMap.find( id );
	if ( iter == m_infoMap.end() )
		return nullptr;

	return &(iter->second);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AddInfo
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool CharacterInfoManagerTemplate::_AddInfo(
	    AtInt32 id,
	    AtBool isNonUse,
	    Protocol::EBagType bagType )
{
	auto iter = m_infoMap.find( id );
	if ( iter != m_infoMap.end() )
		return false;

	CharacterInfo info = CharacterInfo(
	    id,
	    isNonUse,
	    bagType );

	m_infoMap[ id ] = info;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool CharacterInfoManagerTemplate::_Initialize()
{
	if ( !_AddInfo( 0, 0, Protocol::EBagType::Equipment ) ) return false;
	if ( !_AddInfo( 2, 0, Protocol::EBagType::Useable ) ) return false;

	return true;
}