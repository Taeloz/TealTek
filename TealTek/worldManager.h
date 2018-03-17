#pragma once
#include <vector>
#include <SDL.h>
#include "header.h"
#include "player.h"
#include "sprite.h"
#include "worldMap.h"

using namespace std;

class worldManager
{
private:
	// The world map
	worldMap *currentMap;

	// Array of sprite objects
	vector<Sprite> *objects;

	// Array of textures in the tilemap
	SDL_Texture *tilemap[5];

	// Array of sprites
	SDL_Texture *sprites[2];

	// Pointer to the maptexture that will be used to render
	SDL_Texture *mapTexture;

	//angle of the ray (in radians)
	double rayAngle, sinAngle, cosAngle, tanAngle;

	// Calculate distance to the projection screen
	double distToProj = SCREEN_WIDTH / 2 / tan(FOV / 2 * M_PI / 180.0);

	//raycasting variables
	int hit, side, mapX, mapY;
	double deltaDistH, deltaDistV, distToH, distToV;
	double Hx, Hy, Vx, Vy;
	int wallX, wallY;
	int imageColumn;
	double wallDist, floorDist, sliceHeight;
	SDL_Rect sliceRect, textureRect, mainViewport;
	double zBuffer[SCREEN_WIDTH];

	int wallMap[20][20];
	int ceilingMap[20][20];
	int floorMap[20][20];

public:
	worldManager(worldMap *map, vector<Sprite> &obj);

	void worldManager::setMap(worldMap *map, vector<Sprite> &obj);
	void worldManager::loadTextures(SDL_Renderer *renderer);

	void render(SDL_Renderer *renderer, player *mainplayer, bool ceilingExists, SDL_Color skyColor);

	//void skyCast();//not implemented
	void wallCast(SDL_Renderer *renderer, double Px, double Py, double direction, int column);
	void floorCast(SDL_Renderer *renderer, double Px, double Py, double direction, int column);
	//void pitCast();//not implemented
	void spriteCast(SDL_Renderer *renderer, double Px, double Py, double direction);
};