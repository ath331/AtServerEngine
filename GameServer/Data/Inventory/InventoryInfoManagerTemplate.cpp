////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InventoryInfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include "InventoryInfoManagerTemplate.h"
#include "InventoryInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief GetInfo
////////////////////////////////////////////////////////////////////////////////////////////////////
const InventoryInfo* InventoryInfoManagerTemplate::GetInfo( AtInt32 id )
{
	auto iter = m_infoMap.find( id );
	if ( iter == m_infoMap.end() )
		return nullptr;

	return &(iter->second);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AddInfo
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool InventoryInfoManagerTemplate::_AddInfo(
	    AtInt32 id,
	    AtBool isNonUse,
	    Protocol::EBagType bagType )
{
	auto iter = m_infoMap.find( id );
	if ( iter != m_infoMap.end() )
		return false;

	InventoryInfo info = InventoryInfo(
	    id,
	    isNonUse,
	    bagType );

	m_infoMap[ id ] = info;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool InventoryInfoManagerTemplate::_Initialize()
{
	if ( !_AddInfo( 0, 0, Protocol::EBagType::Equipment ) ) return false;
	if ( !_AddInfo( 2, 0, Protocol::EBagType::Useable ) ) return false;

	return true;
}