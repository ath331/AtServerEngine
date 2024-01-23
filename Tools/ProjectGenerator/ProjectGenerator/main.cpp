/*
	Copyright 2021, Won Seong-Yeon. All Rights Reserved.
		KoreaGameMaker@gmail.com
		github.com/GameForPeople
*/

/*
	#0. VS2019의 프로젝트 파일, 프로젝트 필터파일에 대하여 현재 디렉토리 구조에 맞게 재생산해주는 툴입니다.

	!0. 프로젝트가 있는 경로를 첫번째 명령 인수로 전달해줘야합니다. ( 프로젝트와 필터가 있어야 만들 수 있습니다. )
	!1. 프로젝트가 있는 경로에 프로젝트 파일(.vcxproj)이 하나라면 추가 명령 인수가 필요없습니다.
	!2. 주어진 경로에 프로젝트가 여러개 있으면 정상적으로 동작하지 않습니다. ( 프로젝트 파일이 누구껀지 알기 어렵습니다. )

	?0. 출력 프로젝트 위치를 바꾸는 작업이 필요하다면
	?1. 입력 프로젝트 위치를 바꾸는 작업이 필요하다면
*/

#ifndef WONSY_PCH

#include <iostream>

#include <string>
#include <sstream>

#include <fstream>
#include <filesystem>

#include <vector>
#include <map>

#endif

enum class FILE_TYPE 
{ 
	Header,  // .h, .hpp
	Source,  // .cpp
	Xml,     // .xml
	Cc,      // .cc
	Proto,   // .proto
	Bat,     // .bat
};

using PathCont = std::vector< std::filesystem::path >;

void Log( const std::string& logStr, const bool isTerminate = true )
{
	std::cout << " [WARN] "<< logStr << std::endl;

	if ( isTerminate )
		std::terminate();
}

std::string FindAndReplace( std::string str, const std::string& before, const std::string& after )
{
    size_t startPos = 0;
    while( ( startPos = str.find( before, startPos ) ) != std::string::npos )
    {
        str.replace( startPos, before.length(), after );
        startPos += after.length();
    }
    return str;
}
#define FAR( str ) FindAndReplace( FindAndReplace( str, rootString, "" ), "/", "\\" ) // 이 무슨;;

int main( int argc, char *argv[] /* 1. 프로젝트 파일이 존재하는 경로 */ )
{
	// Print UI
	{
		std::cout << ""                                                     << std::endl;
		std::cout << "Project Generator ver 0.1"                            << std::endl; 
		std::cout << "Copyright 2021, Won Seong-Yeon. All Rights Reserved." << std::endl;
		std::cout << "		KoreaGameMaker@gmail.com"                       << std::endl;
		std::cout << "		github.com/GameForPeople"                       << std::endl;
		std::cout << "" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "" << std::endl;
	}

	// 명령형 인수에 따라, 프로젝트 경로를 구합니다.
	const std::string rootString = [ argc, &argv ]() -> std::string
		{
			if ( argc < 2 ) 
			{
				argv[ 1 ] = (char *)("geneTestProject/geneTestProject/");
				//argv[ 2 ] = (char *)("geneTestProject"); 
				//argc = 3;
				Log( "주어진 명령형 인수가 없어, 하드 코딩된 default 경로를 사용하였습니다.", false );
			}
			return static_cast< std::string >( argv[ 1 ] );
		}();  
	const std::filesystem::path rootPath = rootString; // Root 경로

	// 소스 링크 검사
	if ( !std::filesystem::exists( rootPath ) )
	{
		Log( "해당 경로가 유효하지 않아 종료합니다." + rootString );
	}
	
	// 경로를 돌아다니며, 프로젝트 이름을 찾는다.
	const std::string projectName = argc >= 3 
		? argv[ 2 ]
		: [ &rootPath ]() -> std::string
		{
			for ( const auto& pathIter : std::filesystem::directory_iterator( rootPath ) )
			{
				if ( 
					pathIter.is_regular_file() &&
					pathIter.path().extension().generic_string() == ".vcxproj" )
				{
					return pathIter.path().stem().generic_string();
				}
			}

			Log( "뭐여 프로젝트 파일 없으면 죽어야지?" );
			return ""; // for warnning
		}();

	const std::filesystem::path projectPath          = rootPath.generic_string() + projectName + ".vcxproj";         // 제너레이트할 프로젝트 경로
	const std::filesystem::path filtersPath          = rootPath.generic_string() + projectName + ".vcxproj.filters"; // 제너레이트할 프로젝트 필터 경로
	const std::string           itemgroupHeadString  = "  <ItemGroup>";
	const std::string           itemgroupTailString  = "  </ItemGroup>";
	// const std::string endString            = "\r\n";

	PathCont                        directoryPathCont; // 해당 경로에 존재하는 모든 디렉토리 경로 모음
	std::map< FILE_TYPE, PathCont > filePathCont;      // 해당 경로에 존재하는 모든 파일 경로 -모음

	// 해당 경로에 존재하는, 모든 폴더들을 필터로 제작하고, 파일들의 모든 경로를 임포트한다.
	{
		for ( const auto& pathIter : std::filesystem::recursive_directory_iterator( rootPath ) ) 
		{
			if ( pathIter.is_directory() )
			{
				directoryPathCont.emplace_back( pathIter.path() );
			}
			else if ( pathIter.is_regular_file() )
			{
				if (
					pathIter.path().extension().generic_string() == ".h" ||
					pathIter.path().extension().generic_string() == ".hpp" )
				{
					filePathCont[ FILE_TYPE::Header ].emplace_back( pathIter.path() );
				}
				else if ( pathIter.path().extension().generic_string() == ".cpp" )
				{
					filePathCont[ FILE_TYPE::Source ].emplace_back( pathIter.path() );
				}
				else if ( pathIter.path().extension().generic_string() == ".xml" )
				{
					filePathCont[ FILE_TYPE::Xml ].emplace_back( pathIter.path() );
				}
				else if ( pathIter.path().extension().generic_string() == ".cc" )
				{
					filePathCont[ FILE_TYPE::Cc ].emplace_back( pathIter.path() );
				}
				else if ( pathIter.path().extension().generic_string() == ".proto" )
				{
					filePathCont[ FILE_TYPE::Proto ].emplace_back( pathIter.path() );
				}
				else if ( pathIter.path().extension().generic_string() == ".bat" )
				{
					filePathCont[ FILE_TYPE::Bat ].emplace_back( pathIter.path() );
				}
			}
		}
	}

	// vcxproj파일을 처리합니다.
	{
		std::cout << " [INFO] Write " << projectName << " Vcxproj   Start ";

		// 현재 있는 프로젝트의 앞, 뒷 부분을 그대로 사용하기 위해 먼저 기존에 있던 프로젝트 파일을 읽고 쓰는 방식으로 진행합니다.
		enum class READ_STATE{ Before, NotUse_Check_1, NotUse_Check_2, After };

		READ_STATE                 readState{ READ_STATE::Before };
		std::vector< std::string > preStringCont;
		std::vector< std::string > postStringCont;

		// 기존에 있던 프로젝트 파일을 읽습니다.
		{
			std::ifstream projectFileStream( projectPath );
			if ( !projectFileStream.is_open() )
			{
				Log( "프로젝트 파일 안열리면 죽어야지?" );
			}

			std::string lineBuffer{};
			while ( projectFileStream )
			{
				// 한줄씩 읽어 처리한다.
				std::getline( projectFileStream, lineBuffer );

				if ( readState == READ_STATE::Before )
				{
					if ( itemgroupHeadString == lineBuffer )
						readState = READ_STATE::NotUse_Check_1;
					else 
						preStringCont.emplace_back( lineBuffer );
				}
				else if ( readState == READ_STATE::NotUse_Check_1 )
				{
					if ( itemgroupTailString == lineBuffer )
					{
						readState = READ_STATE::NotUse_Check_2;
					}
				}
				else if ( readState == READ_STATE::NotUse_Check_2 )
				{
					if ( itemgroupHeadString == lineBuffer )
					{
						readState = READ_STATE::NotUse_Check_1;
					}
					else
					{
						postStringCont.emplace_back( lineBuffer );
						readState = READ_STATE::After;
					}
				}
				else if ( readState == READ_STATE::After )
				{
					// 마지막 문장 EOF때문인지, 2번들어감. 중복 문자 들어가지 않도록 예외처리
					if ( *( --postStringCont.end() ) != lineBuffer )
						postStringCont.emplace_back( lineBuffer );
				}
			}
		}

		// 읽은 파일들을 바탕으로 프로젝트 파일을 생성한다.
		{
			std::ofstream fos{ projectPath };

			// Write Pre
			{
				for ( const auto& preString : preStringCont )
				{
					fos
						<< preString << std::endl;
				}
			}

			// Write Item
			{
				for ( const auto& filePathIter : filePathCont )
				{
					fos
						<< itemgroupHeadString << std::endl;

					if ( filePathIter.first == FILE_TYPE::Header )
					{
						for ( const auto& filePath : filePathIter.second )
						{
							fos
							<< "    <ClInclude Include=\"" << FAR( filePath.relative_path().generic_string() ) << "\" />" << std::endl;
						}
					}
					else
					{
						for ( const auto& filePath : filePathIter.second )
						{
							fos
							<< "    <ClCompile Include=\"" << FAR( filePath.relative_path().generic_string() ) << "\" />" << std::endl;
						}
					}

					fos
						<< itemgroupTailString << std::endl;
				}
			}

			// Write Post
			{
				for ( 
					auto afterStringIter = postStringCont.begin();
					afterStringIter != ( --postStringCont.end() );
					++afterStringIter )
				{
					fos
						<< *afterStringIter << std::endl;
				}

				{
					fos
						<< *( --postStringCont.end() );
				}
			}
		}

		std::cout << " ----------------------------> End! \n";
	}

	{}

	// Filters파일을 처리합니다.
	{
		std::cout << " [INFO] Write " << projectName << " Filters   Start ";

		// 현재 있는 프로젝트 필터 파일의 앞, 뒷 부분을 그대로 사용하기 위해 먼저 기존에 있던 프로젝트 필터 파일을 읽고 쓰는 방식으로 진행합니다.
		std::vector< std::string > preStringCont;
		std::vector< std::string > postStringCont;

		// 멍청하지만 다 읽은 후, 위의 2줄 맨 아래 1줄 뺴온다. -> 포맷 바뀌면 멍청이코드됩니다.
		{
			std::ifstream filtersFileStream( filtersPath );
			if ( !filtersFileStream.is_open() )
			{
				Log( "필터 파일 안열리면 죽어야지?" );
			}

			std::vector< std::string > stringCont;

			std::string lineBuffer{};
			while ( filtersFileStream )
			{
				std::getline( filtersFileStream, lineBuffer );
				stringCont.emplace_back( lineBuffer );
			}

			preStringCont.emplace_back( *stringCont.begin()       );
			preStringCont.emplace_back( *( ++stringCont.begin() ) );

			postStringCont.emplace_back( *( --stringCont.end() ) );
		}

		// 파일을 실제로 씁니다.
		{
			std::ofstream fos{ filtersPath };

			// head
			{
				for ( const auto& preString : preStringCont )
				{
					fos
						<< preString << std::endl;
				}
			}

			// Files
			{
				for ( const auto& filePathIter : filePathCont )
				{
					fos
						<< itemgroupHeadString << std::endl;
					
					for ( const auto& filePath : filePathIter.second )
					{
						if ( filePathIter.first == FILE_TYPE::Header )
						{
							// .h
							fos
								<< "    <ClInclude Include=\"" << FAR( filePath.relative_path().generic_string() ) << "\">" << std::endl
								<< "      <Filter>" << FAR( filePath.parent_path().generic_string() ) << "</Filter>" << std::endl
								<< "    </ClInclude>" << std::endl;
						}
						else if ( filePathIter.first == FILE_TYPE::Source || filePathIter.first == FILE_TYPE::Cc )
						{
							// .cpp
							fos
								<< "    <ClCompile Include=\"" << FAR( filePath.relative_path().generic_string() ) << "\">" << std::endl
								<< "      <Filter>" << FAR( filePath.parent_path().generic_string() ) << "</Filter>" << std::endl
								<< "    </ClCompile>" << std::endl;
						}
						else if ( filePathIter.first == FILE_TYPE::Xml )
						{
							// .xml
							fos
								<< "    <Xml Include=\"" << FAR( filePath.relative_path().generic_string() ) << "\">" << std::endl
								<< "      <Filter>" << FAR( filePath.parent_path().generic_string() ) << "</Filter>" << std::endl
								<< "    </Xml>" << std::endl;
						}
						else if ( filePathIter.first == FILE_TYPE::Proto || filePathIter.first == FILE_TYPE::Bat )
						{
							// .proto
							fos
								<< "    <None Include=\"" << FAR( filePath.relative_path().generic_string() ) << "\">" << std::endl
								<< "      <Filter>" << FAR( filePath.parent_path().generic_string() ) << "</Filter>" << std::endl
								<< "    </None>" << std::endl;
						}
					}

					fos
						<< itemgroupTailString << std::endl;
				}
			}
			
			// Folders
			{
				if ( directoryPathCont.size() )
				{
					fos
						<< itemgroupHeadString << std::endl;

					for ( const auto& directoryPath : directoryPathCont )
					{
						fos
						<< "    <Filter Include=\"" << FAR( directoryPath.generic_string() ) << "\">" << std::endl
						<< "    </Filter>"                                                                                         << std::endl;
					}

					fos
						<< itemgroupTailString << std::endl;
				}
			}

			// Fin
			{
				for ( const auto& postString : postStringCont )
				{
					fos
						<< postString; // << '\0';
				}
			}
		}

		std::cout << " ----------------------------> End! \n";
	}
}