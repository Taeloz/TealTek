#pragma once
#include <string>
#include <iostream>
#include "tealTek.h"
#include "header.h"
#include "sprite.h"

tealTek::tealTek() : world(currentMap, objects)
{

}

tealTek::~tealTek()
{
	delete currentMap;
	delete mainPlayer;
}

bool tealTek::run()
{
	bool running = true;

	// Variables for FPS counter
	Uint32 startTime = SDL_GetTicks();
	int numFrames = 0;

	while (running)
	{
		Uint32 startFrame = SDL_GetTicks();

		getInput(running);

		world.render(renderer, mainPlayer, currentMap->ceilingExists, currentMap->skyColor);
		SDL_UpdateWindowSurface(window);

		//FPS counter
		numFrames++;
		std::cout << "\r" << std::string(50, ' ') << "\r";
		std::cout << (float)numFrames / (SDL_GetTicks() - startTime) * 1000.0;
		if (SDL_GetTicks() - startTime > 2000.0) {
			startTime = SDL_GetTicks();
			numFrames = 0;
		}
		
		//FPS limiter
		int frameTime = SDL_GetTicks() - startFrame;
		if (frameTime < 1000 / FPS_CAP) { SDL_Delay(1000 / FPS_CAP - frameTime); }
		
	}

	return true;
}

bool tealTek::init(std::string mapFileName)
{
	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Create the world map and player instances
	currentMap = new worldMap(mapFileName);
	mainPlayer = new player(currentMap->startPosition.x, currentMap->startPosition.y, currentMap->startAngle);

	world.setMap(currentMap, objects);

	// Initialize window and renderer
	window = SDL_CreateWindow("TealTek 1.0", 300, 100, SCREEN_WIDTH, static_cast<int>(SCREEN_HEIGHT * 1.2), SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

	// Load sprites
	loadSprites();

	// Load textures
	world.loadTextures(renderer);	

	return true;
}

bool tealTek::shutdown()
{
	SDL_Quit();

	return true;
}

void tealTek::getInput(bool &running)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
				break;
			}
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// TODO: Implement player width (right now player is a point and can move forward along a wall closer than if hitting the wall

	//Strafe right
	if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RIGHT]) {
		double destX = mainPlayer->posX + SPEED * cos((mainPlayer->direction + 90) * M_PI / 180) + 100 * cos((mainPlayer->direction + 90) * M_PI / 180);
		double destY = mainPlayer->posY + SPEED * -sin((mainPlayer->direction + 90) * M_PI / 180) + 100 * -sin((mainPlayer->direction + 90) * M_PI / 180);
		if (currentMap->getWall((int)(destX / RES), (int)(mainPlayer->posY / RES)) == 0)
		{
			mainPlayer->posX = destX - 100 * cos((mainPlayer->direction + 90) * M_PI / 180);
		}
		if (currentMap->getWall((int)((mainPlayer->posX) / RES), (int)(destY / RES)) == 0)
		{
			mainPlayer->posY = destY - 100 * -sin((mainPlayer->direction + 90) * M_PI / 180);
		}
		//Strafe left
	}
	else if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_LEFT]) {
		double destX = mainPlayer->posX + SPEED * cos((mainPlayer->direction - 90) * M_PI / 180) + 100 * cos((mainPlayer->direction - 90) * M_PI / 180);
		double destY = mainPlayer->posY + SPEED * -sin((mainPlayer->direction - 90) * M_PI / 180) + 100 * -sin((mainPlayer->direction - 90) * M_PI / 180);
		if (currentMap->getWall((int)(destX / RES), (int)(mainPlayer->posY / RES)) == 0)
		{
			mainPlayer->posX = destX - 100 * cos((mainPlayer->direction - 90) * M_PI / 180);
		}
		if (currentMap->getWall((int)((mainPlayer->posX) / RES), (int)(destY / RES)) == 0)
		{
			mainPlayer->posY = destY - 100 * -sin((mainPlayer->direction - 90) * M_PI / 180);
		}
	}
	else {
		//Turn left or right
		if (state[SDL_SCANCODE_RIGHT]) mainPlayer->direction += SPEED / 4;
		if (state[SDL_SCANCODE_LEFT]) mainPlayer->direction -= SPEED / 4;

		if (mainPlayer->direction < 0) mainPlayer->direction += 360;
		if (mainPlayer->direction > 360) mainPlayer->direction -= 360;
	}

	//Move forward
	if (state[SDL_SCANCODE_UP]) {
		double destX = mainPlayer->posX + SPEED * cos(mainPlayer->direction * M_PI / 180) + 100 * cos(mainPlayer->direction * M_PI / 180);
		double destY = mainPlayer->posY + SPEED * -sin(mainPlayer->direction * M_PI / 180) + 100 * -sin(mainPlayer->direction * M_PI / 180);
		if (currentMap->getWall((int)((destX) / RES), (int)(mainPlayer->posY / RES)) == 0)
		{
			mainPlayer->posX = destX - 100 * cos(mainPlayer->direction * M_PI / 180);
		}
		if (currentMap->getWall((int)((mainPlayer->posX) / RES), (int)(destY / RES)) == 0)
		{
			mainPlayer->posY = destY - 100 * -sin(mainPlayer->direction * M_PI / 180);
		}
	}

	//Move backward
	if (state[SDL_SCANCODE_DOWN]) {
		double destX = mainPlayer->posX - SPEED * cos(mainPlayer->direction * M_PI / 180) - 100 * cos(mainPlayer->direction * M_PI / 180);
		double destY = mainPlayer->posY - SPEED * -sin(mainPlayer->direction * M_PI / 180) - 100 * -sin(mainPlayer->direction * M_PI / 180);
		if (currentMap->getWall((int)(destX / RES), (int)(mainPlayer->posY / RES)) == 0)
		{
			mainPlayer->posX = destX + 100 * cos(mainPlayer->direction * M_PI / 180);
		}
		if (currentMap->getWall((int)((mainPlayer->posX) / RES), (int)(destY / RES)) == 0)
		{
			mainPlayer->posY = destY + 100 * -sin(mainPlayer->direction * M_PI / 180);
		}

	}
}

void tealTek::loadSprites() 
{

	for (int i = 0; i < NUM_SPRITES; i++) {
		Sprite *n = new Sprite;
		objects.push_back(*n);
	}
	objects[0].posX = 5 * RES + RES / 2;
	objects[0].posY = 9 * RES + RES / 2;

	objects[1].posX = 7 * RES + RES / 2;
	objects[1].posY = 9 * RES + RES / 2;

	objects[2].posX = 11 * RES + RES / 2;
	objects[2].posY = 8 * RES + RES / 2;

	objects[3].posX = 11 * RES + RES / 2;
	objects[3].posY = 6 * RES + RES / 2;
}