////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagers ( Auto Make File )
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include <iostream>
#include "Logic/Utils/Log/AtLog.h"
{%- for InfoManagerNameAndPath in InfoManagerNameAndPaths %}
#include "Data/{{InfoManagerNameAndPath}}InfoManager.h"
{%- endfor %}


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagers
////////////////////////////////////////////////////////////////////////////////////////////////////
AtBool InitializeInfoManager()
{
	INFO_LOG( "InitializeInfoManager Start" );


{%- for InfoManagerName in InfoManagerNames %}
	if ( !{{InfoManagerName}}InfoManager::GetInstance().Initialize() ) return false;
{%- endfor %}


	INFO_LOG( "InitializeInfoManager End" );

	return true;
}



