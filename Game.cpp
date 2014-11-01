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
	
	TextureAtlas* ta = sdl->loadAtlas( "atlas.png", "atlas.txt" );
	level = new Level();
	level->load( "level1.txt", ta );
	player = new Player("player", "mario.png");
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
	// check scene tiles around player
	player->contactingGround = false;
	for( int i = -1; i <= 1; i++ )
	{
		for( int j = 1 ; j >= -1 ; j-- )
		{
			int x = player->rect.x + i * Level::tileSize;
			int y = player->rect.y + j * Level::tileSize;
			char c = level->tileNear( x, y );
			RectF tileRect = level->tileRectNear( x, y );
			sdl->fillRect( tileRect, Blue );
			
			if( c && c != '.' )
			{
				Vector2f overlaps = player->rect.offsetsToClear( tileRect );
				if( j == -1 )
				{
					// tiles above
				}
				else if( j == 0 )
				{
					// tile left and right
					player->rect.x += overlaps.x;
					sdl->fillRect( tileRect, Yellow );
				}
				else
				{
					// tiles below
					if( overlaps.y )
					{
						player->rect.y += overlaps.y - EPS;
						player->contactingGround = true;
						sdl->fillRect( tileRect, Red );
					}
				}
			}
		}
	}

	//if( player->contactingGround )
	//	puts( "Ground contact" );
	//else
	//	puts( "air time" );
}

void Game::runGame()
{
	// Use the controller state to change gamestate
	if( controller.keystate[ SDL_SCANCODE_UP ] )
		player->jump();
	if( controller.keystate[ SDL_SCANCODE_RIGHT ] )
		player->accel( +1 );
	if( controller.keystate[ SDL_SCANCODE_LEFT ] )
		player->accel( -1 );
	
	player->update();
	
	sdl->origin.x = player->rect.x - sdl->getSize().x/2;
	sdl->origin.y = player->rect.y - sdl->getSize().y/2;

	// Check for collisions after each object moves
	//checkForCollisions();

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

void Game::clear()
{
	// Set the background's color,
	sdl->setColor( bkgColor );

	// clears the bkg to bkgColor
	SDL_RenderClear( sdl->renderer );
	
}

void Game::draw()
{
	if( gameState == Title )
	{
		title->draw();
	}
	else
	{
		player->draw();

		// draw the background sprites, but only the tiles that are visible
		// 
		int startX = sdl->origin.x;
		clamp( startX, 0, level->getSizeX()*32 );

		int endX = startX + sdl->getSize().x + 32;
		clamp( endX, 0, level->getSizeX()*32 );

		int startY = sdl->origin.y;
		clamp( startY, 0, level->getSizeY()*32 );
		
		int endY = startY + sdl->getSize().y;
		clamp( endY, 0, level->getSizeY()*32 );
		//printf( "Render from x=%d to x=%d, y=%d to y=%d\n", startX, endX, startY, endY );
		for( int x = startX; x < endX; x+=32 )
		{
			for( int y = startY; y < endY; y+=32 )
			{
				char c = level->tileNear( x, y );

				// no tile there
				if( !c )	c='-';//skip;
				if( c=='.' ) skip;// air
				
				string spriteName = level->spriteMap[c];

				// blank, nothing gets drawn for these
				if( spriteName.empty() )	skip;
				
				// starting pt to start drawing tiles depends on player start location
				RectF rect = level->tileRectNear( x, y );
				sdl->drawAtlasSpriteAt( rect.x, rect.y, "atlas.png", spriteName );
			}
		}
	}
	
	if( gameState == Paused )
	{
		pausedText->draw();
	}
	
	checkForCollisions();

	SDL_RenderPresent( sdl->renderer );
}

