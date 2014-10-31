#ifndef SPRITE_H
#define SPRITE_H

#include "SDLO.h"
#include "Vectorf.h"
#include "RectF.h"

class Game;

class Sprite
{
public:
	// Eq of singletons, but not singleton.
	static SDL* sdl;		// The SDL object, for rendering
	static Game* game;		// The Game object, which contains all game world objects

	// object name, for debugging purposes
	string name;
	
	// I keep a RECTANGLE to represent the sprite.
	// The sprite's position is in the rectangle's xy()
	RectF rect;
	
	// This is the velocity of the sprite.
	Vector2f vel;
	
	// Color of the object
	SDL_Color color;

	// Texture, if any
	SDL_Texture* tex;
	
	bool hidden;

	void defaults();
	Sprite(string iname);
	Sprite(string iname, SDL_Texture* iTex);
	Sprite(string iname, string filename);
	static Sprite* Text(TTF_Font* font, string text, SDL_Color iColor);
	Vector2f getPos();
	Vector2f getCenter();
	void setPos( Vector2f pos );
	void setPos( float x, float y )
	{
		setCenter(Vector2f(x,y));
	}
	void setCenter( Vector2f pos );
	void setCenter( float x, float y )
	{
		setCenter(Vector2f(x,y));
	}
	void setSize( Vector2f size );
	void scale( float s );
	void boundTB();
	void boundLR();
	void show();
	void hide();
	virtual void update();
	virtual void draw();
	
private:
	void retrieveTexSize();
};

#endif