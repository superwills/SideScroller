#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

#include "RectF.h"
#include "Tile.h"

class TextureAtlas;

class Level
{
public:
	// maps 
	vector<string> levelMap;
	vector< vector<Tile> > tiles;

	// maps characters to strings
	map<char, string> spriteMap;
	// a reference to the texture atlas used for bkg sprites
	TextureAtlas *ta;
	const static int tileSize = 32;
	
	Level();
	char tileCharNear( int x, int y );
	Tile tileNear( int x, int y );
	Tile tileAbove( int x, int y );
	Tile tileBelow( int x, int y );
	Tile tileLeftOf( int x, int y );
	Tile tileRightOf( int x, int y );
	bool isImpassible( Tile tile );
	bool load( string filename, TextureAtlas *texAtlas );
	int getSizeX();
	int getSizeY();
};

#endif