#pragma once
#include <string>
#include <vector>

struct Vector2
{
	int x;
	int y;
};

class worldMap
{
private:
	int wallMap[20][20];
	int ceilingMap[20][20];
	int floorMap[20][20];

	std::string mapName;

	int tileset;
	int skyImage;
	

public:
	Vector2 startPosition;
	double startAngle;
	SDL_Color skyColor;
	bool ceilingExists;

	worldMap(std::string name)
	{
		mapName = name;
		tileset = 0;
		skyImage = 0;

		//memset(wallMap, 0, sizeof(wallMap));
		int placeholderwallmap[20][20] = {
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
			{ 1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
			{ 1,0,2,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,2,2,0,0,0,1 },
			{ 1,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1 },
			{ 1,0,2,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1 },
			{ 1,0,0,0,1,0,0,0,0,0,1,2,2,2,0,0,2,2,2,1 },
			{ 1,1,1,1,1,0,0,0,0,0,1,2,0,0,0,0,0,0,2,1 },
			{ 1,1,1,1,1,1,0,1,0,1,1,2,0,0,0,0,0,0,2,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,0,0,2,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,2,2,0,0,0,0,2,2,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,2,2,0,0,2,2,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
		memcpy(wallMap, placeholderwallmap, sizeof(placeholderwallmap));

		int placeholderfloormap[20][20] = {
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
			{ 1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1 },
			{ 1,0,2,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1 },
			{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,2,2,0,0,0,1 },
			{ 1,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1 },
			{ 1,0,2,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1 },
			{ 1,0,0,0,1,0,0,0,0,0,1,2,2,2,3,3,2,2,2,1 },
			{ 1,1,1,1,1,0,0,0,0,0,1,2,3,3,3,3,3,3,2,1 },
			{ 1,1,1,1,1,1,0,1,0,1,1,2,3,3,3,3,3,3,2,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,2,3,3,3,3,3,3,2,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,2,2,3,3,3,3,2,2,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,2,2,3,3,2,2,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
		memcpy(floorMap, placeholderfloormap, sizeof(placeholderfloormap));

		int placeholderceilingmap[20][20] = {
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,4,4,4,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1 },
			{ 1,4,4,4,1,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1 },
			{ 1,4,2,4,4,4,4,4,4,4,1,1,4,4,4,4,4,4,4,1 },
			{ 1,4,4,4,1,4,4,4,4,4,4,4,4,4,2,2,4,4,4,1 },
			{ 1,4,4,4,1,4,4,4,4,4,1,1,4,4,4,4,4,4,4,1 },
			{ 1,4,2,4,4,4,4,4,4,4,1,1,1,1,4,4,1,1,1,1 },
			{ 1,4,4,4,1,4,4,4,4,4,1,1,4,4,4,4,4,4,1,1 },
			{ 1,1,1,1,1,4,4,4,4,4,1,1,4,4,4,4,4,4,1,1 },
			{ 1,1,1,1,1,1,4,1,4,1,1,1,4,4,4,4,4,4,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,4,4,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
		memcpy(ceilingMap, placeholderceilingmap, sizeof(placeholderceilingmap));

		startPosition.x = 7;
		startPosition.y = 5;
		startAngle = 180;

		ceilingExists = false;
		skyColor = { 125, 90, 75 };
	}

	int getTileset()
	{
		return tileset;
	}

	int getWall(int x, int y)
	{
		return wallMap[x][y];
	}

	int getCeiling(int x, int y)
	{
		return ceilingMap[x][y];
	}

	int getFloor(int x, int y)
	{
		return floorMap[x][y];
	}

};