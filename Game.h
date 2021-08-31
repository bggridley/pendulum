#pragma once
#include <SDL.h>
#include <glew.h>
#include <SDL_image.h>
#include <iostream>
#include "BitmapFont.h"
#include "Texture.h"
#include "Renderer.h"
#include "Resources.h"



//class Item; // we're not going to save explicit different items so we don't need multiple forward declarations like with GuiPaused et
class Item;
class Inventory;
class World;
class GuiMainMenu;
class GuiPaused;
class Gui;

class Game {
public:
	Game();
	~Game();
	Texture loadTexture(std::string path, GLint param);
	SDL_Window* window;
	Resources resources;


	void init(); // this is necessary so that textures can be created
	void input(SDL_Event* event);
	void update(float delta, int updateCount, int frameCount);
	int updates;
	void drawSquiggle(float x, float y, float angle, float scale);
	void render(float alpha);
	void setDimensions(float width, float height);
	void setFullscreenDimensions(float width, float height);
	float getWidth();
	float getHeight();
	float getScale();
	BitmapFont getFont();
	Renderer* getRenderer();
	Resources getResources();

	GLuint pbo;
	GLuint fbo, render_buf, normal_fbo;

	std::vector<float> pointArray;

	float iw = 1280;// 3840;//1280;// 3840;
	float ih = 720;//3840;// 720;// 3840;
	float mass1 = 40;
	float mass2 = 40;

	float length1 = 250.0f; // 125
	float length2 = 180.0f; // 180

	float sx = iw / 2;
	float sy = ih / 2;

	float theta1 = 3.14f;
	float theta2 = 0.0f;

	float px = 0.0f;
	float py = 0.0f;

	float pa = 0.0f;
	float pv = 0.0f;

	float theta1_v = 0.0f; // angular velocity
	float theta2_v = 0.0f;

	float g = 19.81f;

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
	Renderer renderer;
	BitmapFont font;
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