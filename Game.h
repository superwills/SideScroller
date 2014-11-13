#ifndef GAME_H
#define GAME_H

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "SDLO.h"
#include "Sprite.h"
#include "Controller.h"
#include "TitleScreen.h"
#include "Level.h"
#include "Player.h"

class Game
{
public:
	enum GameState { Title, Running, Paused, Exiting };
	
private:
	// The SDL object, which allows us to draw and play sounds
	SDL* sdl;
	
	// The 3 game objects we have in the game
	TitleScreen* title;
	Sprite* pausedText;
	SDL_Color bkgColor;	// the current background color
	
	Level* level;
	Player* player;

	// The font we use to draw the scores
	TTF_Font* font;

	// what mode the game is running in
	GameState gameState;
	
	// An object to represent the game controller
	Controller controller;

	Vector2f camPos;
	
public:
	Game( SDL& iSdl );
	~Game();
	GameState getState();
	// change fsm state
	void setState( GameState newState );
	void togglePause();
	void drawScores();
	void checkForCollisions();
	void runGame();
	void update();
	void clear();
	void draw();
};

#endif