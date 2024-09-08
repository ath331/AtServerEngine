////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InventoryInfoManagerManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"
#include "Logic/Utils/Log/AtLog.h"
#include "InventoryInfo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InventoryInfoManagerTemplate class
////////////////////////////////////////////////////////////////////////////////////////////////////
class InventoryInfoManagerTemplate
{
protected:
	/// InventoryInfoMap
	using InventoryInfoMap = std::map< AtInt32, InventoryInfo >;

protected:
	/// InventoryInfoMap
	InventoryInfoMap m_infoMap;

public:
	/// GetInfo
	const InventoryInfo* GetInfo( AtInt32 id );

private:
	/// AddInfo
	AtBool _AddInfo(
	    AtInt32 id,
	    AtBool isNonUse,
	    Protocol::EBagType bagType );

protected:
	/// Initialize
	AtBool _Initialize();
};