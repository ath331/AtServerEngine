////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief CharacterInfoManager class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"
#include "CharacterInfoManagerTemplate.h"
#include "Core/Singleton.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief CharacterInfoManager class
////////////////////////////////////////////////////////////////////////////////////////////////////
class CharacterInfoManager
	:
	public CharacterInfoManagerTemplate,
	public Singleton< CharacterInfoManager >
{
public:
	/// Initialize
	AtBool Initialize();
};