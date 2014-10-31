#include "SDLO.h"
#include "Sprite.h"

// ctor requires just window width & height
SDL::SDL(const char* title, int windowWidth, int windowHeight):window(0), screenSurface(0), renderer(0),
size(windowWidth, windowHeight)
{
	Sprite::sdl = this;
	
	if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
	{
		error( "SDL init error: %s\n", SDL_GetError() );
		exit( 1 );
	}

	window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		windowWidth, windowHeight, SDL_WINDOW_SHOWN );
	if( !window )
	{
		error("Window could not be created! error: %s\n", SDL_GetError());
		exit( 1 );
	}
	
	// retrieve the surface (collection of pixels) of the window
	screenSurface = SDL_GetWindowSurface( window );
	
	// create a hardware accelerated renderer
	// that displays only once per refresh interval
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	// Turn on alpha blending
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
}

SDL::~SDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void SDL::setColor( SDL_Color color )
{
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
}

// Draws you a simple line
void SDL::line( int startX, int startY, int endX, int endY, SDL_Color color )
{
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
	SDL_RenderDrawLine( renderer, startX, startY, endX, endY );
}

void SDL::rect( int x, int y, int w, int h, SDL_Color color )
{
	SDL_Rect rect = { x, y, w, h };
	setColor( color );
	SDL_RenderDrawRect( renderer, &rect );
}

void SDL::fillRect( int x, int y, int w, int h, SDL_Color color )
{
	SDL_Rect rect = { x, y, w, h };
	setColor( color );
	SDL_RenderFillRect( renderer, &rect );
}

void SDL::playMusic( string musicFile )
{
	/// stop all other music
	Mix_HaltMusic();

	if( musics.find(musicFile) == musics.end() )
	{
		loadMusic( musicFile );
	}

	Mix_PlayMusic( musics[musicFile], -1 );
}

void SDL::playSound( string soundFile )
{
	if( sfx.find(soundFile) == sfx.end() )
	{
		warning( "Sound %s not yet loaded!", soundFile.c_str() );
		loadWavSound( soundFile );
	}

	Mix_PlayChannel( -1, sfx[soundFile], 0 ) ;
}

SDL_Surface* SDL::loadSurface( string filename )
{
	SDL_Surface* opSurface = NULL;
	SDL_Surface* rawSurface = IMG_Load( filename.c_str() );
	if( !rawSurface )
	{
		error( "Couldn't load `%s`! Error: %s\n", filename.c_str(), IMG_GetError() );
	}
	else
	{
		// Convert the loaded surface to same as screen surface format
		opSurface = SDL_ConvertSurface( rawSurface, screenSurface->format, NULL );
		if( !opSurface )
		{
			error( "Unable to optimize image %s! Error: %s\n", filename.c_str(), SDL_GetError() );
		}
		SDL_FreeSurface( rawSurface );
	}
	return opSurface;
}

SDL_Texture* SDL::loadTexture( string filename )
{
	map<string,SDL_Texture*>::iterator iter = texes.find( filename );
	if( iter != texes.end() )
	{
		warning( "%s already loaded", filename.c_str() );
		return iter->second;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface( renderer, loadSurface(filename) );
	SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
	texes[ filename ] = tex;
	return tex;
}

SDL_Texture* SDL::makeText( TTF_Font* font, string text, SDL_Color color )
{
    SDL_Surface *messagesurf = TTF_RenderText_Solid( font, text.c_str(), color );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, messagesurf );
	SDL_FreeSurface( messagesurf );
	return texture;
}

Mix_Music* SDL::loadMusic( string filename )
{
	if( musics.find(filename) != musics.end() )
	{
		error( "Music %s already loaded", filename.c_str() );
		return 0;
	}
	Mix_Music *music = Mix_LoadMUS( filename.c_str() ) ;
	musics[filename] = music;
	return music;
}

Mix_Chunk* SDL::loadWavSound( string waveFilename )
{
	if( sfx.find(waveFilename) != sfx.end() )
	{
		error( "Sound `%s` already loaded", waveFilename.c_str() );
		return sfx[waveFilename];
	}

	Mix_Chunk *sound = Mix_LoadWAV( waveFilename.c_str() ) ;
	sfx[waveFilename] = sound;
	return sound;
}
