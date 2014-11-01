#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <sdl.h>
#include <map>
#include <string>
using namespace std;

class SDL;

class TextureAtlas
{
public:
	// The rectangles of sprites by name inside the texture atlas.
	map<string, SDL_Rect> spriteRects;
	SDL_Texture* tex;
	
	TextureAtlas();
	bool loadData( string datafile );
};

#endif