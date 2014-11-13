#ifndef COLORS_H
#define COLORS_H

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

extern SDL_Color White, Black, Red, Green, Blue, Yellow;

inline SDL_Color SDL_ColorMake( Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
	SDL_Color color = { r,g,b,a };
	return color;
}

#endif