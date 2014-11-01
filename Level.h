#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

#include "RectF.h"

class TextureAtlas;

class Level
{
public:
	// maps 
	vector<string> levelMap;
	// maps characters to strings
	map<char, string> spriteMap;
	// a reference to the texture atlas used for bkg sprites
	TextureAtlas *ta;
	const static int tileSize = 32;
	
	Level();
	char tileNear( int x, int y );
	RectF tileRectNear( int x, int y );
	bool load( string filename, TextureAtlas *texAtlas );
	int getSizeX();
	int getSizeY();
};

#endif