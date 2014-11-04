#include "Player.h"

Player::Player(string name, string imageFile) : Sprite(name, imageFile)
{
	contactingGround = false;
}

void Player::jump()
{
	if( contactingGround )
	{
		vel.y += -10;
		sdl->playSound( "jump.wav" );
	}	
}

void Player::bounceDown()
{
	vel.y = -vel.y;
}

void Player::accel( float amt )
{
	vel.x += amt;
	movingLeft = amt < 0;
}

void Player::update()
{
	vel.y += .085;//gravity
	clamp<float>( vel.y, -5.f, 5.f );
	rect.xy() += vel ;
	vel.x *= 0.97;
	clamp<float>( vel.x, -5.f, 5.f );
}

