////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief TestInfoManagerManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"
#include "Logic/Utils/Log/AtLog.h"
#include "TestInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief TestInfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class TestInfoManagerTemplate
{
protected:
	/// TestInfoMap
	using TestInfoMap = std::map< AtInt32, TestInfo >;

protected:
	/// TestInfoMap
	TestInfoMap m_infoMap;

public:
	/// GetInfo
	const TestInfo* GetInfo( AtInt32 id );

private:
	/// AddInfo
	AtBool _AddInfo(
	    AtInt32 id,
	    Protocol::EActorType type,
	    AtString string );

protected:
	/// Initialize
	AtBool _Initialize();
};