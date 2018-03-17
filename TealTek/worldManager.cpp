#pragma once
#include <vector>
#include <algorithm> // for std::sort
#include "worldManager.h"

using namespace std;

worldManager::worldManager(worldMap *map, vector<Sprite> &obj)
{
	currentMap = map;
	objects = &obj;
}

// Sets the world map and objects to be used
void worldManager::setMap(worldMap *map, vector<Sprite> &obj)
{
	currentMap = map;
	objects = &obj;
}

// Render a screen
void worldManager::render(SDL_Renderer *renderer, player *mainplayer, bool ceilingExists, SDL_Color skyColor = { 0, 0, 0 })
{
	// Player position
	double Px = mainplayer->posX;
	double Py = mainplayer->posY;

	// Clear the screen to prepare for rendering
	SDL_SetRenderDrawColor(renderer, 25, 25, 50, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	// Rects containing area for sky and ground if not rendering with textures
	if (ceilingExists == false)
	{
		SDL_Rect sky = { 0, 0, SCREEN_WIDTH,SCREEN_HEIGHT / 2 };
		SDL_SetRenderDrawColor(renderer, skyColor.r, skyColor.g, skyColor.b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &sky);
	}

	std::sort(objects->begin(), objects->end(), [](Sprite a, Sprite b) { return a.spriteDist > b.spriteDist; });

	// Loop through each column on the screen
	for (int column = 0; column < SCREEN_WIDTH; column++)
	{
		wallCast(renderer, Px, Py, mainplayer->direction, column);
		floorCast(renderer, Px, Py, mainplayer->direction, column);
	}

	spriteCast(renderer, Px, Py, mainplayer->direction);

	//spriteCast(renderer, Px, Py, mainplayer->direction);

	mainViewport.x = 0;
	mainViewport.y = 0;
	mainViewport.w = SCREEN_WIDTH;
	mainViewport.h = SCREEN_HEIGHT;

	//Render screen
	SDL_RenderSetViewport(renderer, &mainViewport);
}

// Cast a ray for a particlar column of the screen given player position and direction
void worldManager::wallCast(SDL_Renderer *renderer, double Px, double Py, double direction, int column)
{
	//angle of this ray in radians
	rayAngle = direction * M_PI / 180.0 - atan((SCREEN_WIDTH / 2 - column) / distToProj); ///Correction for variable ray angles

	///Old Method
	//rayAngle = (direction + ((FOV * column / SCREEN_WIDTH) - FOV/2) * M_PI / 180.0;

	// Calculate the trigonometric functions once for this ray
	sinAngle = sin(rayAngle);
	cosAngle = cos(rayAngle);
	tanAngle = sinAngle / cosAngle;

	//Find first point of horizontal intersection at current ray angle
	Hy = (sinAngle < 0) ? floor(Py / RES) * RES + RES : floor(Py / RES) * RES - 0.001;
	Hx = Px + (Py - Hy) / tanAngle;

	//Find first point of vertical intersection at current ray angle
	Vx = (cosAngle < 0) ? floor(Px / RES) * RES - 0.001 : floor(Px / RES) * RES + RES;
	Vy = Py + (Px - Vx) * tanAngle;

	//Distance change between horizonal and vertical intersections at the current ray angle
	deltaDistH = fabs(RES / sinAngle);
	deltaDistV = fabs(RES / cosAngle);

	//Distance to first points of intersection
	distToH = sqrt((Px - Hx) * (Px - Hx) + (Py - Hy) * (Py - Hy));
	distToV = sqrt((Px - Vx) * (Px - Vx) + (Py - Vy) * (Py - Vy));

	hit = 0; // Sentinel value to break out of following loop when a wall is hit
	side = 0; // Value to determine whether horizontal or vertical wall was hit
	imageColumn = 0; // Column in texture to render to column on screen

	// Follow the ray until it hits a wall
	while (hit == 0)
	{
		// Choose the shortest distance to the next grid intersection
		if (distToH <= distToV)
		{
			// Check for a hit
			if (currentMap->getWall((int)(Hx / RES), (int)(Hy / RES)) > 0)
			{
				hit = 1;
				side = 0;
				mapX = (int)(Hx / RES);
				mapY = (int)(Hy / RES);
				wallDist = distToH;
				imageColumn = (int)(fabs(Hx - (int)(Hx / RES)*RES));
				wallX = (int)Hx;
				wallY = (int)Hy;
			}
			else { // No hit means send the ray another step
				distToH += deltaDistH;
				Hx += deltaDistH * cosAngle;
				Hy += deltaDistH * -sinAngle;
			}
		}
		else {
			// Check for a hit
			if (currentMap->getWall((int)(Vx / RES), (int)(Vy / RES)) > 0)
			{
				hit = 1;
				side = 1;
				mapX = (int)(Vx / RES);
				mapY = (int)(Vy / RES);
				wallDist = distToV;
				imageColumn = (int)(fabs(Vy - (int)(Vy / RES)*RES));
				wallX = (int)Vx;
				wallY = (int)Vy;
			}
			else { // No hit means send the ray another step
				distToV += deltaDistV;
				Vx += deltaDistV * cosAngle;
				Vy += deltaDistV * -sinAngle;
			}
		}
	} // end when hit is found

	/** At this point in the raycast:
	**		mapX and MapY contain the map coordinates of wall hit
	**		wallX and wallY contain the pixel coordinates of hit location in map
	**		wallDist contains the distance to the wall
	**		side = 0 if horizontal side of wall is hit
	**		side = 1 if vertical side of wall is hit
	**/

	// Store the distance to the wall slice in the zbuffer for later use when drawing sprites
	zBuffer[column] = wallDist;

	///Old fisheye correction. Does not account for variable angles between rays.
	///wallDist *= cos(((FOV * column / SCREEN_WIDTH) - FOV/2)* M_PI / 180.0);

	// New fisheye correction
	wallDist *= cos(atan((SCREEN_WIDTH / 2 - column) / distToProj));

	// Prevent a divide by zero or really large numerator
	wallDist = (wallDist < 0.001) ? 0.001 : wallDist;

	// Find height of slice to draw on screen
	sliceHeight = (RES / floor(wallDist) * (distToProj));

	// Get the texture corresponding to the wallmap and get mapTexture to point to it
	mapTexture = tilemap[currentMap->getWall(mapX, mapY)];

	// TODO: Improve environment effects
	// Darken the color if a vertical wall is hit
	if (side == 1) {
		SDL_SetTextureColorMod(mapTexture, 128, 128, 128);
	}
	else {
		SDL_SetTextureColorMod(mapTexture, 255, 255, 255);
	}

	//Get the slice from the screen to render to
	int y = (int)((SCREEN_HEIGHT / 2) - (sliceHeight / 2));
	int height = (int)(sliceHeight);
	sliceRect = { column, y, 1, height };

	// Get the slice of the texture being rendered from
	textureRect = { imageColumn, 0, 1, (int)(RES) };

	// Render the wall slice
	SDL_RenderCopy(renderer, mapTexture, &textureRect, &sliceRect);
}

// Cast the floor for a particular column of the screen
void worldManager::floorCast(SDL_Renderer *renderer, double Px, double Py, double direction, int column)
{
	// Restore any color darkening before rendering the floor
	SDL_SetTextureColorMod(mapTexture, 255, 255, 255);
	
	// The row to start drawing the floor for this specific column
	int startRow = (SCREEN_HEIGHT / 2) + (int)(sliceHeight / 2);

	// Angle for fisheye correction
	double betaAngle = 1 / cos(atan((SCREEN_WIDTH / 2 - column) / distToProj));

	// Loop through each pixel in the column under the wall slice
	for (int row = startRow; row < SCREEN_HEIGHT; row++)
	{
		// Distance to the floor pixel
		floorDist = (RES / 2 * distToProj) / (row - (SCREEN_HEIGHT / 2));

		// Fisheye correction
		floorDist *= betaAngle;

		// Find map coordinate of the floor tile being hit
		int currentMapX = (int)fabs(Px + floorDist * cosAngle) / RES;
		int currentMapY = (int)fabs(Py + floorDist * -sinAngle) / RES;

		// Find the pixel coordinates on the texture to render from
		int currentFloorX = (int)fabs(Px + floorDist * cosAngle) % RES;
		int currentFloorY = (int)fabs(Py + floorDist * -sinAngle) % RES;

		// Get the slices from the screen to render to (1px by 1px)
		sliceRect = { column, row, 1, 1 };

		// Get the slice of the texture to render from (1px by 1px)
		textureRect = { currentFloorX, currentFloorY, 1, 1 };

		// Get the actual texture for the current tile
		mapTexture = tilemap[currentMap->getFloor(currentMapX, currentMapY)];

		//Render the floor pixel
		SDL_RenderCopy(renderer, mapTexture, &textureRect, &sliceRect);

		//Render the ceiling
		if (currentMap->getCeiling(currentMapX, currentMapY) > 0)
		{
			mapTexture = tilemap[currentMap->getCeiling(currentMapX, currentMapY)];
			sliceRect = { column, SCREEN_HEIGHT - row, 1, 1 };
			SDL_RenderCopy(renderer, mapTexture, &textureRect, &sliceRect);
		}
	}
}

void worldManager::spriteCast(SDL_Renderer *renderer, double Px, double Py, double direction) 
{
	for (int i = 0; i < NUM_SPRITES; i++)
	{
		objects->at(i).calculateDist(Px, Py);
	}

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		Sprite *thisSprite = &objects->at(i);

		//double angleToSprite = (-asin(objects[i].posY - Py)/acos(objects[i].posX - Px)) * 180 / M_PI;
		double angleToSprite = atan2(thisSprite->posX - Px, thisSprite->posY - Py);
		angleToSprite = angleToSprite * 180 / M_PI + 270;
		angleToSprite = (angleToSprite < 0) ? angleToSprite + 360 : angleToSprite;
		angleToSprite = (angleToSprite > 360) ? angleToSprite - 360 : angleToSprite;

		double startView = direction + 80;
		startView = (startView < 0) ? startView + 360 : startView;
		startView = (startView > 360) ? startView - 360 : startView;
		double endView = direction - 80;
		endView = (endView < 0) ? endView + 360 : endView;
		endView = (endView > 360) ? endView - 360 : endView;

		if (endView - startView > 120 && angleToSprite > endView)
		{
			startView += 360;
		}
		if (endView - startView > 120 && angleToSprite < startView)
		{
			endView -= 360;
		}

		if (angleToSprite > endView && angleToSprite < startView)
		{
			if (angleToSprite - direction > 180) direction += 360;
			if (angleToSprite - direction < -180) angleToSprite += 360;

			int spriteColumn = (int)((SCREEN_WIDTH / FOV) * (angleToSprite - direction + FOV / 2));
			thisSprite->spriteDist *= cos(atan((SCREEN_WIDTH / 2 - spriteColumn) / distToProj));

			thisSprite->spriteDist = (thisSprite->spriteDist <= 0) ? 0.1 : thisSprite->spriteDist;

			double spriteHeight = (double)RES / thisSprite->spriteDist * distToProj;
			double spriteWidth = (double)RES / thisSprite->spriteDist * distToProj;

			int startColumn = (int)(spriteColumn - spriteWidth / 2);
			int endColumn = (int)(spriteColumn + spriteWidth / 2);

			double j = 0;
			if (startColumn < 0) {
				j = fabs(startColumn) * RES / spriteWidth;
				startColumn = 0;
			}
			if (endColumn > SCREEN_WIDTH) endColumn = SCREEN_WIDTH;

			for (int col = startColumn; col <= endColumn; col++)
			{
				if (thisSprite->spriteDist < zBuffer[col])
				{
					thisSprite->spriteRect = { (int)j, 0, 1, 256 };
					thisSprite->screenRect = { col, (int)(SCREEN_HEIGHT / 2 - spriteHeight / 2), 1, (int)spriteHeight };
					SDL_RenderCopy(renderer, sprites[0], &thisSprite->spriteRect, &thisSprite->screenRect);
				}
				j += ((double)RES / spriteWidth);
			}
		}
	}
}

// Load the textures
void worldManager::loadTextures(SDL_Renderer *renderer)
{
	// 0
	SDL_Surface *temp;
	temp = SDL_LoadBMP("Textures/floor.bmp");
	if (temp == nullptr)
	{
		printf(SDL_GetError());
	}
	tilemap[0] = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	// 1
	temp = SDL_LoadBMP("Textures/wall.bmp");
	tilemap[1] = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	// 2
	temp = SDL_LoadBMP("Textures/RedBrickWall.bmp");
	tilemap[2] = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	// 3
	temp = SDL_LoadBMP("Textures/ConcreteFloor.bmp");
	tilemap[3] = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	// 4
	temp = SDL_LoadBMP("Textures/grid.bmp");
	tilemap[4] = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

	// Load sprite textures
	temp = SDL_LoadBMP("Textures/tree.bmp");
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 255, 255));
	sprites[0] = SDL_CreateTextureFromSurface(renderer, temp);
	//SDL_SetTextureBlendMode(sprites[0], SDL_BLENDMODE_BLEND);
	//SDL_SetTextureAlphaMod(sprites[0], 180);
	SDL_FreeSurface(temp);
}