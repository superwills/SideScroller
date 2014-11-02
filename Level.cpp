#include "Level.h"
#include "StdWilUtil.h"
#include "TextureAtlas.h"
#include <sstream>
using namespace std;

Level::Level()
{
}

char Level::tileCharNear( int x, int y )
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

Tile Level::tileNear( int x, int y )
{
	Tile tile;
	tile.c = tileCharNear( x, y );
	tile.rect.x = x/tileSize*tileSize;
	tile.rect.y = y/tileSize*tileSize;
	tile.rect.w = tileSize;
	tile.rect.h = tileSize;
	return tile;
}

Tile Level::tileAbove( int x, int y )
{
	return tileNear( x, y - tileSize );
}

Tile Level::tileBelow( int x, int y )
{
	return tileNear( x, y + tileSize );
}

Tile Level::tileLeftOf( int x, int y )
{
	return tileNear( x - tileSize, y );
}

Tile Level::tileRightOf( int x, int y )
{
	return tileNear( x + tileSize, y );
}

bool Level::isImpassible( Tile tile )
{
	return tile.c && tile.c != '.';
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
	
	// output the map to the console
	//for( int i = 0; i < levelMap.size(); i++ )
	//	printf( "%s\n", levelMap[i].c_str() );
	return true;
}

int Level::getSizeX()
{
	// # cols in each row
	if( !levelMap.size() )
	{
		error( "No map" );
		return 0;
	}
	
	return levelMap[0].size();
}

int Level::getSizeY()
{
	// # rows
	return levelMap.size();
}