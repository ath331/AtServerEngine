////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Environment File
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>


////////////////////////////////////////////////////////////////////////////////////////////////////
// @brief Environment Class
////////////////////////////////////////////////////////////////////////////////////////////////////
class Environment
{
private:
	static std::map< AtString, AtString > data;

	Environment() {}

public:
	// Function to load .ini file and parse it into the map
	static AtBool Load( const AtString& filename )
	{
		std::ifstream file( filename );
		if ( !file.is_open() )
		{
			std::cerr << "Could not open file: " << filename << std::endl;
			return false;
		}

		AtString line;
		while ( std::getline( file, line ) )
		{
			// Ignore comments and empty lines
			if ( line.empty() || line[ 0 ] == '#' )
				continue;

			std::istringstream iss( line );
			AtString key, value;

			// Use '=' as delimiter
			if ( std::getline( iss, key, '=' ) && std::getline( iss, value ) )
			{
				// Remove possible whitespaces around the key and value
				key = Trim( key );
				value = Trim( value );

				// Store the key-value pair in the map
				data[ key ] = value;
			}
		}

		file.close();
		return true;
	}

	// Static function to get value by key
	static AtString Get( const AtString& key )
	{
		auto it = data.find( key );
		if ( it != data.end() )
		{
			return it->second;
		}
		else
		{
			return ""; // Return empty string if key is not found
		}
	}

private:
	// Helper function to trim whitespace from a string
	static AtString Trim( const AtString& str )
	{
		const AtString whitespace = " \t\n\r";
		size_t start = str.find_first_not_of( whitespace );
		size_t end = str.find_last_not_of( whitespace );
		return ( start == AtString::npos || end == AtString::npos )
			? "" : str.substr( start, end - start + 1 );
	}
};
