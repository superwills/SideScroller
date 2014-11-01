#include "Player.h"

Player::Player(string name, string imageFile) : Sprite(name, imageFile)
{
	
}

void Player::accel( float amt )
{
	vel.x += amt;
}

void Player::update()
{
	rect.x += vel.x ;
	vel.x *= 0.97;
}
