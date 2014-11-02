#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"

class Player : public Sprite
{
public:
	bool contactingGround;
	Player(string name, string imageFile);
	
	void jump();
	void bounceDown();
	void accel( float amt );

	void update();
};

#endif