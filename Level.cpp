#include "Level.h"
#include "StdWilUtil.h"
#include "TextureAtlas.h"
#include <sstream>
using namespace std;

Level::Level()
{
}

bool Level::load(string filename, TextureAtlas *texAtlas)
{
	ta = texAtlas;
	
	// Open the filestream
	ifstream in( filename.c_str() );
	if( !in )
	{
		error( "Could not open level %s!", filename.c_str() );
		return false;
	}

	// Repeatedly get lines until eof
	string line;
	enum ReadingMode
	{
		ReadingLegend,
		ReadingData
	};
	ReadingMode readingMode = ReadingLegend;

	while( !in.eof() )
	{
		getline( in, line );
		if( !line.size() || line[0] == '#' )
		{
			// skip empty lines & comments
			skip;
		}
		
		// change readingMode if encounter changer
		if( line == "legend:" )
		{
			readingMode = ReadingLegend;
		}
		else if( line == "data:" )
		{
			readingMode = ReadingData;
		}
		else
		{
			// The line is not a mode changer.. so we parse it
			if( readingMode == ReadingLegend )
			{
				istringstream istream( line );
				// first token is the char key
				char c;
				string name;
				// take c and name from istream
				istream >> c;
				istream >> name;
				spriteMap[c] = name;
				printf( "Legend entry: `%c` => `%s`\n", c, name.c_str() );
			}
			else
			{
				levelMap.push_back( line );
			}
		}
	}
	
	// out put the map to the console
	//for( int i = 0; i < levelMap.size(); i++ )
	//	printf( "%s\n", levelMap[i].c_str() );
	return true;
}

