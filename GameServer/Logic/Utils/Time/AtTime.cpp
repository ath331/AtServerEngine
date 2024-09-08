////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtTime File
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "pch.h"
#include "AtTime.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief 현재 시스템 시간을 문자열 포맷으로 반환한다.
////////////////////////////////////////////////////////////////////////////////////////////////////
AtString AtTime::GetCurTimeFormat( AtString format )
{
	auto now = std::chrono::system_clock::now();

	std::time_t currentTime = std::chrono::system_clock::to_time_t( now );

	std::tm* timeInfo = std::localtime( &currentTime );
	std::stringstream timeStream;
	timeStream << std::put_time( timeInfo, format.c_str() );

	return timeStream.str();
}
