////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "pch.h"
#include "InfoManagerTemplate.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief 持失切
////////////////////////////////////////////////////////////////////////////////////////////////////
InfoManagerTemplate::InfoManagerTemplate()
{
	m_id = AtInt32( 0 );
	m_type = Protocol::EActorType::None;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief 社瑚切
////////////////////////////////////////////////////////////////////////////////////////////////////
InfoManagerTemplate::~InfoManagerTemplate()
{
}
