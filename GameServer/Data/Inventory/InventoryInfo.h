////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InventoryInfo class
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Packet/Protocol.pb.h"
#include "InventoryInfoTemplate.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief InventoryInfo class
////////////////////////////////////////////////////////////////////////////////////////////////////
class InventoryInfo
	:
	public InventoryInfoTemplate
{
public:
	/// Constructor
	InventoryInfo();

	/// Constructor
	InventoryInfo(
	    AtInt32 id,
	    AtBool isNonUse,
	    Protocol::EBagType bagType );

	/// Destructor
	~InventoryInfo();
};

/// SharedPointer
using InventoryInfoPtr = std::shared_ptr< InventoryInfo >;