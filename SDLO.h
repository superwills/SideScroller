#ifndef SDLO_H
#define SDLO_H

#include <map>
using namespace std;

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_image.h>
#include <SDL2_Mixer/SDL_mixer.h>
#include <SDL2_TTF/SDL_ttf.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#endif

#include "TextureAtlas.h"
#include "Log.h"
#include "RectF.h"

// If you link the SDL libraries here, you don't
// have to worry about setting the project settings
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_image.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )

// Stores everything to do with SDL, and game assets
class SDL
{
	// asset maps: filename=>SDL_* objects
	map<string, SDL_Texture*> texes;

	// you can have multiple texture atlases.
	map<string, TextureAtlas*> texAtlases;
	map<string, Mix_Music*> musics;
	map<string, Mix_Chunk*> sfx;
	Vector2f size;

	
public:
	// Screen origin. We subtract the origin from every point drawn
	// to get simple 2d camera motion (panning).
	Vector2f origin;

	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;

	// ctor requires just window width & height
	SDL(const char *title, int windowWidth, int windowHeight);
	~SDL();
	
	inline Vector2f getSize()
	{
		return size;
	}
	void setColor( SDL_Color color );

	// Draws you a simple line
	void line( int startX, int startY, int endX, int endY, SDL_Color color );
	void rect( int x, int y, int w, int h, SDL_Color color );
	void rect( RectF rect, SDL_Color color );
	void fillRect( int x, int y, int w, int h, SDL_Color color );
	void fillRect( RectF rect, SDL_Color color );
	void drawTextureFlippedX( SDL_Texture* tex, SDL_Rect rect );
	void drawTexture( SDL_Texture* tex, SDL_Rect rect );
	void drawAtlasSpriteAt( int x, int y, string atlasName, string spriteName );

	void playMusic( string musicFile );
	void playSound( string soundFile );
	
	SDL_Surface* loadSurface( string filename );
	SDL_Texture* loadTexture( string filename );
	TextureAtlas* loadAtlas( string imagefile, string datafile );
	SDL_Texture* makeText( TTF_Font* font, string text, SDL_Color color );
	Mix_Music* loadMusic( string filename );
	Mix_Chunk *loadWavSound( string waveFilename );
};

#endif