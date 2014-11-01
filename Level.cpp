#include "Level.h"
#include "StdWilUtil.h"
#include "TextureAtlas.h"
#include <sstream>
using namespace std;

Level::Level()
{
}

char Level::tileNear( int x, int y )
{
	int i = y / tileSize;
	int j = x / tileSize;
	
	if( i >= levelMap.size() )
	{
		//error( "i=%d is OOB", i );
		return 0;
	}
	else if( j >= levelMap[i].size() )
	{
		//error( "j=%d is OOB", j );
		return 0;
	}

	return levelMap[i][j];
}

RectF Level::tileRectNear( int x, int y )
{
	RectF rect;
	rect.x = x/tileSize*tileSize;
	rect.y = y/tileSize*tileSize;
	rect.w = tileSize;
	rect.h = tileSize;
	return rect;
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

int Level::getSizeX()
{
	if( !levelMap.size() )
	{
		error( "No map" );
		return 0;
	}

	return levelMap[0].size();
}

int Level::getSizeY()
{
	return levelMap.size();
}