#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <sdl.h>

// handles user input and modifies the game state based on that
class Controller
{
public:
	int mouseX, mouseY;
	const Uint8 *keystate;
	Controller();
	// Updates the controller -- call once per frame
	void update();
};

#endif