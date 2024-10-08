////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtTime File
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtTime Class
////////////////////////////////////////////////////////////////////////////////////////////////////
class AtTime
{
public:
	// 현재 시스템 시간을 문자열 포맷으로 반환한다.
	static AtString GetCurTimeFormat( AtString format = "%Y-%m-%d %H:%M:%S" );
};