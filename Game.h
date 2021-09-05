#pragma once
#include <SDL.h>
#include <glew.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include "DoublePendulum.h"

class Game {
public:
	Game();
	~Game();
	
	SDL_Window* window;

	DoublePendulum* pendulum;
	//DoublePendulum* pendulum;
	void init(); // this is necessary so that textures can be created
	void printScreen(int i);
	void input(SDL_Event* event);
	void update(float delta, int updateCount, int frameCount);
	void updateLogic(float delta, int updateCount);
	void updateMany(int updates, float deltaTime);
	int updates;
	void drawSquiggle(float x, float y, float angle, float scale);
	void render(float alpha);
	int getWidth() { return 200; }
	int getHeight() { return 200;  }

	GLuint pbo;
	GLuint fbo, render_buf, normal_fbo;

	float iw = 3840;// 1920;// 3840;//1280;// 3840;// 1280;// 3840;//1280;// 3840;
	float ih = 3840;// 1080;// 3840;// 720;// 3840;// 720;// 3840;// 720;// 3840;

	bool updatem = false;

	float sx = iw / 2;
	float sy = ih / 2;

	enum GameState {
		MAIN_MENU,
		INGAME
	};


	int mouseX;
	int mouseY;
	bool zoomIn = false;
	bool zoomOut = false;
	bool mousePressed = false;

private:
	int state;
	int fps;


	float scale = 1.0f;
	float width;
	float height;
	float fullscreenWidth;
	float fullscreenHeight;

	

	
	bool vsync = true;
	bool fullscreen = false;

};