#pragma once
#include <math.h>
#include "sprite.h"

Sprite::Sprite() 
{
	posX = 4 * RES + RES / 2;
	posY = 7 * RES + RES / 2;
}

void Sprite::calculateDist(double Px, double Py) 
{
	spriteDist = sqrt((Px - posX) * (Px - posX) + (Py - posY) * (Py - posY));
}