#pragma once
#include <string>
#include "sprite.h"
#include "worldManager.h"
#include "player.h"

class tealTek
{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	worldManager world;

	worldMap *currentMap = nullptr;
	player *mainPlayer = nullptr;

	std::vector<Sprite> objects;
	SDL_Texture *sprites[1];

public:
	tealTek();
	~tealTek();
	bool init(std::string mapFileName);
	bool run();
	bool shutdown();
	
	void getInput(bool &running);

	//void render();
	void loadSprites();

};