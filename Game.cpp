#include "Game.h"
#include "Colors.h"

Game::Game( SDL& iSdl )
{
	// Link up the static reference to this Game object
	// inside Sprite:
	Sprite::game = this;
	sdl = &iSdl;
	gameState = Title;

	// Start up true-type fonts
	TTF_Init();
	font = TTF_OpenFont("Nobile-Bold.ttf", 128);
	if(!font)
	{
		error("TTF_OpenFont: %s", TTF_GetError());
	}
	
	// Start up the audio mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		error( "SDL_mixer Error: %s\n", Mix_GetError() );
	}
	
	// Create game sprites
	bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
	title = new TitleScreen( "title", "title.png" );
	pausedText = Sprite::Text( font, "pause", SDL_ColorMake( 200, 200, 0, 200 ) );
	pausedText->setCenter( sdl->getSize()/2 );

	// load the sfx. These sfx were created with SFXR
	// http://www.drpetter.se/project_sfxr.html
	sdl->loadWavSound( "ping0.wav" );
	sdl->loadWavSound( "ping1.wav" );
	sdl->loadWavSound( "ping2.wav" );
	sdl->loadWavSound( "ping3.wav" );
	sdl->loadWavSound( "win.wav" );
	
	setState( Title );
}

Game::~Game()
{
	
}

Game::GameState Game::getState()
{
	return gameState;
}

void Game::setState( GameState newState )
{
	GameState prevState = gameState;
	switch( newState )
	{
	case Title:
		// start the title music
		sdl->playMusic( "song 91 7.ogg" );
	 	break;

	case Running:
		// game song
		// if prevstate was Running, don't restart the music
		if( prevState == Paused )
		{
			Mix_ResumeMusic(); // unpause the music
		}
		else // start playback of the game music
		{
			sdl->playMusic( "song 81 6.flac" );
		}
		break;

	case Paused:
		Mix_PauseMusic();
		break;

	case Exiting:
		break;
	}

	gameState = newState;
}

void Game::togglePause()
{
	if( gameState == Paused )
	{
		setState( Running );
		pausedText->hide();
	}
	else
	{
		setState( Paused );
		pausedText->show();
	}
}

void Game::checkForCollisions()
{

}

void Game::runGame()
{
	// Use the controller state to change gamestate

	// Check for collisions after each object moves
	checkForCollisions();
}

void Game::update()
{
	// Get controller inputs first:
	controller.update();

	if( gameState == Title )
	{
		title->update();
	}
	else if( gameState == Running )
	{
		runGame();
	}
}

void Game::draw()
{
	// Set the background's color,
	sdl->setColor( bkgColor );

	// clears the bkg to bkgColor
	SDL_RenderClear( sdl->renderer );
	
	if( gameState == Title )
	{
		title->draw();
	}
	else
	{
		// draw the game
	}
	
	if( gameState == Paused )
	{
		pausedText->draw();
	}
	
	SDL_RenderPresent( sdl->renderer );
}

