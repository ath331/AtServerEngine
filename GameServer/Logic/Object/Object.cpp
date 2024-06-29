////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif Object class
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "Object.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif 생성자
////////////////////////////////////////////////////////////////////////////////////////////////////
Object::Object()
{
	objectInfo = new Protocol::ObjectInfo();
	posInfo    = new Protocol::PosInfo();

	objectInfo->set_allocated_pos_info( posInfo );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// @breif 소멸자
////////////////////////////////////////////////////////////////////////////////////////////////////
Object::~Object()
{
	// set_allocated_pos_info로 posInfo를 들고 있기때문에 objectInfo가 날아갈때 posInfo도 같이 날아감.
	if ( objectInfo )
	{
		delete objectInfo;
		objectInfo = nullptr;
	}
}