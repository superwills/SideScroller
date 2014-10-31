#include "TitleScreen.h"
#include "Colors.h"

TitleScreen::TitleScreen(string title, string filename):
Sprite(title, filename)
{
	color = SDL_ColorMake(0,0,0,0);
}

void TitleScreen::update()
{
	if( color.r < 255 )
	{
		color.r++;
		color.g++;
		color.b++;
		color.a++;
		SDL_SetTextureColorMod( tex, color.r, color.g, color.b );
	}
}
