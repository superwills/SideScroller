#ifndef TITLE_H
#define TITLE_H

#include "Sprite.h"

class TitleScreen : public Sprite
{
public:
	TitleScreen(string title, string filename);
	void update();
};

#endif