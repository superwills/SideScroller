#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"

class Player : public Sprite
{
public:
	Player(string name, string imageFile);

	void accel( float amt );

	void update();
};

#endif