#pragma once
#include <SDL.h>
#include "header.h"


class Sprite {
private:



public:
	Sprite();

	double posX, posY;
	double direction = 270;
	double spriteDist;
	SDL_Rect spriteRect, screenRect;

	void calculateDist(double Px, double Py);

};