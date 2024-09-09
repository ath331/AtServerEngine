////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtLog File
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once


#define INFO_LOG( msg )    AtLog::PrintMsg( __FILE__, __LINE__, msg );
#define WARNNING_LOG( msg ) AtLog::PrintRedMsg( __FILE__, __LINE__, msg );


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtStringColor Class
////////////////////////////////////////////////////////////////////////////////////////////////////
class AtStringColor
{
public:
	enum class EColor
	{
		None = 0, // 기본값
		Red  = 1, // 빨간색
	};
			
public:
	// 생성자
	AtStringColor( EColor color )
		:
		m_color( color )
	{
		if ( m_color == EColor::None )
			return;

		m_handle = GetStdHandle( STD_OUTPUT_HANDLE );
		if ( m_color == EColor::Red )
			SetConsoleTextAttribute( m_handle, FOREGROUND_RED );
	}

	// 소멸자
	~AtStringColor()
	{
		SetConsoleTextAttribute( m_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	}

private:
	// 핸들 값
	HANDLE m_handle;

	// 현재 컬러
	EColor m_color = EColor::None;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtLog Class
////////////////////////////////////////////////////////////////////////////////////////////////////
class AtLog
{
public:
	// msg를 출력한다.
	static AtVoid PrintMsg( const char* file, AtInt32 line, AtString msg );

	// msg를 빨간색으로 출력한다.
	static AtVoid PrintRedMsg( const char* file, AtInt32 line, AtString msg );

private:
	// 파일 경로에서 파일 이름만 추출하는 함수
	static const char* extractFileName( const char* filePath );
};