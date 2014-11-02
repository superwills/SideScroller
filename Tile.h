#ifndef TILE_H
#define TILE_H

#include "Sprite.h"

class Tile
{
public:
	Tile():c(0){}
	Tile(char ic, RectF& irect);
	char c;
	RectF rect;
};

#endif