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
		None = 0, // �⺻��
		Red  = 1, // ������
	};
			
public:
	// ������
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

	// �Ҹ���
	~AtStringColor()
	{
		SetConsoleTextAttribute( m_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
	}

private:
	// �ڵ� ��
	HANDLE m_handle;

	// ���� �÷�
	EColor m_color = EColor::None;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief AtLog Class
////////////////////////////////////////////////////////////////////////////////////////////////////
class AtLog
{
public:
	// msg�� ����Ѵ�.
	static AtVoid PrintMsg( const char* file, AtInt32 line, AtString msg );

	// msg�� ���������� ����Ѵ�.
	static AtVoid PrintRedMsg( const char* file, AtInt32 line, AtString msg );

private:
	// ���� ��ο��� ���� �̸��� �����ϴ� �Լ�
	static const char* extractFileName( const char* filePath );
};