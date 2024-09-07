////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagers ( Auto Make File )
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include <iostream>
#include "Data/TestInfoManager.h"
#include "Data/Character/CharacterInfoManager.h"
#include "Data/Inventory/InventoryInfoManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagers
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool InitializeInfoManager()
{
	std::cout << "InitializeInfoManager Start" << std::endl;
	if ( !TestInfoManager::GetInstance().Initialize() ) return false;
	if ( !CharacterInfoManager::GetInstance().Initialize() ) return false;
	if ( !InventoryInfoManager::GetInstance().Initialize() ) return false;


	std::cout << "InitializeInfoManager End" << std::endl;

	return true;
}


