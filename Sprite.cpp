#include "Sprite.h"
#include "Game.h"
#include "Colors.h"

SDL* Sprite::sdl;
Game* Sprite::game;

void Sprite::defaults()
{
	color = SDL_ColorMake( 255, 255, 255, 255 );
	hidden = false;
}

Sprite::Sprite(string iname)
{
	defaults();
	name = iname;
	tex = 0;
}

Sprite::Sprite(string iname, SDL_Texture* iTex)
{
	defaults();
	name = iname;
	tex = iTex;
	retrieveTexSize();
}

Sprite::Sprite(string iname, string filename)
{
	defaults();
	name = iname;
	tex = sdl->loadTexture(filename);
	retrieveTexSize();
}

Sprite* Sprite::Text(TTF_Font* font, string text, SDL_Color iColor)
{
	return new Sprite( string("Text ") + text, sdl->makeText( font, text, iColor ) );
}

Vector2f Sprite::getPos()
{
	return rect.xy();
}

void Sprite::setPos( Vector2f pos )
{
	rect.xy() = pos;
}

Vector2f Sprite::getCenter()
{
	return rect.centroid();
}

void Sprite::setCenter( Vector2f pos )
{
	rect.setCenter( pos );
}

void Sprite::setSize( Vector2f size )
{
	rect.size() = size;
}

void Sprite::scale( float s )
{
	rect.size() *= s;
}

void Sprite::boundTB()
{
	if( rect.top() < 0 )
	{
		float overshot = 0 - rect.top();
		rect.y += overshot;
		vel.y = -vel.y; // reflect y vel
	}
	if( rect.bottom() > sdl->getSize().y )
	{
		float overshot = sdl->getSize().y - rect.bottom();
		rect.y += overshot;
		vel.y = -vel.y; // reflect y vel
	}
}

void Sprite::boundLR()
{
	// ensure stays within bounds of world
	// two of these can happen simultaneously, which is why no else is used
	if( rect.left() < 0 )
	{
		float overshot = - rect.left();
		rect.x += overshot;
		vel.x = -vel.x; // reflect x vel
	}
	if( rect.right() > sdl->getSize().x )
	{
		float overshot = sdl->getSize().x - rect.right();
		rect.x += overshot;
		vel.x = -vel.x; // reflect x vel
	}
}

void Sprite::show()
{
	hidden = false;
}

void Sprite::hide()
{
	hidden = true;
}

void Sprite::update()
{
	rect.xy() += vel;
	boundTB();
	boundLR();
}

void Sprite::draw()
{
	if( hidden )
		return ; // just don't draw
	
	sdl->setColor( color );
	if( !tex )
	{
		// no texture, so draw a solid rect
		sdl->fillRect( rect.x, rect.y, rect.w, rect.h, color );
	}
	else
	{
		// Convert our floating pt rect to an int-based rect
		SDL_Rect r = { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };
		SDL_RenderCopy( sdl->renderer, tex, NULL, &r );
	}
}

void Sprite::retrieveTexSize()
{
	int w,h;
	SDL_QueryTexture( tex, 0, 0, &w, &h );
	rect.w=w, rect.h=h;
}
