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
	sdl->loadWavSound( "jump.wav" );
	sdl->loadWavSound( "hitBlock.wav" );
	sdl->loadWavSound( "win.wav" );
	
	TextureAtlas* ta = sdl->loadAtlas( "atlas.png", "atlas.txt" );
	level = new Level();
	level->load( "level1.txt", ta );
	player = new Player("player", "dario.png");
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

	// check tile below
	Vector2f centroid = player->rect.centroid();
	Tile below = level->tileBelow( centroid.x, centroid.y );
	sdl->fillRect( below.rect, Blue );

	if( level->isImpassible( below ) && player->rect.bottom() > below.rect.top() )
	{
		sdl->fillRect( below.rect, Red );
		player->rect.y = below.rect.top() - Level::tileSize ;
		player->contactingGround = true;
		player->vel.y = 0;
	}

	Tile tileAbove = level->tileAbove( centroid.x, centroid.y );
	sdl->fillRect( tileAbove.rect, Blue );

	if( level->isImpassible( tileAbove ) && player->rect.top() < tileAbove.rect.bottom() )
	{
		player->rect.y = tileAbove.rect.bottom()+1;
		player->bounceDown();

		// need to check bottom again
		if( level->isImpassible( below ) && player->rect.bottom() > below.rect.top() )
		{
			player->rect.y = below.rect.top() - Level::tileSize ;
			player->contactingGround = true;
			player->vel.y = 0;
		}
	}
	Tile tileRight = level->tileRightOf( centroid.x, centroid.y );
	sdl->fillRect( tileRight.rect, Blue );
	
	if( level->isImpassible( tileRight ) && player->rect.right() > tileRight.rect.left() )
	{
		player->rect.x = tileRight.rect.x - Level::tileSize;
		player->vel.x = 0;
	}
	Tile tileLeft = level->tileLeftOf( centroid.x, centroid.y );
	sdl->fillRect( tileLeft.rect, Blue );
	if( level->isImpassible( tileLeft ) && player->rect.left() < tileLeft.rect.right() )
	{
		player->rect.x = tileLeft.rect.right()+1;
		player->vel.x = 0;
	}

}

void Game::runGame()
{
	// Use the controller state to change gamestate
	if( controller.keystate[ SDL_SCANCODE_UP ] )
		player->jump();
	if( controller.keystate[ SDL_SCANCODE_RIGHT ] )
		player->accel( +.1 );
	if( controller.keystate[ SDL_SCANCODE_LEFT ] )
		player->accel( -.1 );
	
	player->update();
	
	sdl->origin.x = player->rect.x - sdl->getSize().x/2;
	sdl->origin.y = player->rect.y - sdl->getSize().y/2;

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
		int startX = sdl->origin.x;
		clamp( startX, 0, level->getSizeX()*Level::tileSize );
		int endX = startX + sdl->getSize().x + Level::tileSize;
		clamp( endX, 0, level->getSizeX()*Level::tileSize );
		int startY = sdl->origin.y;
		clamp( startY, 0, level->getSizeY()*Level::tileSize );
		int endY = startY + sdl->getSize().y + Level::tileSize;
		clamp( endY, 0, level->getSizeY()*Level::tileSize );
		
		for( int x = startX; x < endX; x+=Level::tileSize )
		{
			for( int y = startY; y < endY; y+=Level::tileSize )
			{
				char c = level->tileCharNear( x, y );

				// no tile there
				if( !c )	c='-';//skip;
				if( c=='.' ) skip;// air
				
				string spriteName = level->spriteMap[c];

				// blank, nothing gets drawn for these
				if( spriteName.empty() )	skip;
				
				// starting pt to start drawing tiles depends on player start location
				Tile tile = level->tileNear( x, y );
				sdl->drawAtlasSpriteAt( tile.rect.x, tile.rect.y, "atlas.png", spriteName );
			}
		}
	}
	
	if( gameState == Paused )
	{
		pausedText->draw();
	}
	
	//checkForCollisions();//so debug info renders on top
	SDL_RenderPresent( sdl->renderer );
}

