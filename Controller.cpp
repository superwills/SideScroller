#include "Controller.h"

Controller::Controller()
{
	mouseX = mouseY = 0;
	keystate = SDL_GetKeyboardState(NULL);
}

void Controller::update()
{
	SDL_GetRelativeMouseState( &mouseX, &mouseY );
	keystate = SDL_GetKeyboardState(NULL);
}