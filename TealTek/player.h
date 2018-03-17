#pragma once
#include "header.h"
#include "SDL.h"

class player 
{

private:

public:
	player(int startX, int startY, double startDirection)
	{
		posX = startX * RES + RES / 2.0;
		posY = startY * RES + RES / 2.0;
		direction = startDirection;
	}

	double posX, posY;
	double direction;

};