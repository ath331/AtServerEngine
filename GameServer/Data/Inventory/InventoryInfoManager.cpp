////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InventoryInfoManager class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include "InventoryInfoManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool InventoryInfoManager::Initialize()
{
	if ( !_Initialize() )
		return false;

	return true;
}