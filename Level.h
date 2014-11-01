#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

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
	
	Level();
	bool load( string filename, TextureAtlas *texAtlas );
};

#endif