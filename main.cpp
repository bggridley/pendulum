#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Game.h"

int main(int argc, char** argv) {


	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL failed to initialize" << std::endl;
	}
	else {
		std::cout << "SDL initialized successfully" << std::endl;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	else {
		std::cout << "SDL_Image intiialized successfully" << std::endl;
	}


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_Mixer failed to intiialize" << std::endl;
	}
	else {
		std::cout << "SDL_Mixer initialized successfully" << std::endl;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8); // antialiasing

	int display_count = 0, display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	if ((display_count = SDL_GetNumVideoDisplays()) < 1) {
		SDL_Log("SDL_GetNumVideoDisplays returned: %i", display_count);
		return 1;
	}

	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
		return 1;
	}


	Game* game = new Game(); // create a new game instance
	game->setDimensions(mode.w / 1.5, mode.h / 1.5);
	game->setFullscreenDimensions(mode.w, mode.h);
	game->window = SDL_CreateWindow("Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->getWidth(), game->getHeight(), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_SetWindowFullscreen(game->window, 0);
	SDL_GLContext Context = SDL_GL_CreateContext(game->window);

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to intitialize GLEW" << std::endl;
	}
	else {
		std::cout << "GLEW intiailized successfully" << std::endl;
	}

	glewExperimental = GL_TRUE;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, game->getWidth(), game->getHeight(), 0, -1, 1);
	glEnable(GL_MULTISAMPLE);

	// done once, give us a fresh projection matrix.

	//glDisable(GL_DEPTH_TEST);

	GLuint pbo;
//	glGenBuffers(1, &pbo);
//	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
//	glBufferData(GL_PIXEL_PACK_BUFFER, game->iw * game->ih * 4, NULL, GL_DYNAMIC_READ);

	GLuint fbo, render_buf;
	GLuint depthrenderbuffer;
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &render_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, game->iw, game->ih);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, game->iw, game->ih);

	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, game->iw, game->ih);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, game->iw, game->ih);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buf);

	std::cout << ((glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	




	SDL_GL_SetSwapInterval(1);
	game->fbo = fbo;
	game->render_buf = render_buf;
//	game->pbo = pbo;
	game->init();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int curFPS = 0;

	int updates = 0;
	int totalUpdates = 0;
	int frames = 0;
	const int fps = 60;
	float t = 0.0;

	float dt = 1.0f / 60.0f; // TICKINTERVAL
	float deltaBuffer = 0.0f;
	float deltaTime = 0.0f;
	Uint64 currentTime = SDL_GetPerformanceCounter(); // gotta do this so it doesnt blow up
	float accumulator = 0.0f;

	bool left, right, up, down;
	int xf = 1;
	int yf = 1;

	float scale = 1.0f;
	while (true)
	{
	start:

		if (t >= 1.0f) {
			t -= 1.0f;
			std::cout << "FPS: " << frames << " | UPS: " << updates << std::endl;
			curFPS = frames;
			frames = 0;
			updates = 0;
		}

		Uint64 newTime = SDL_GetPerformanceCounter();
		deltaTime = float(newTime - currentTime) / SDL_GetPerformanceFrequency();
		currentTime = newTime;
		//std::cout << deltaTime << std::endl;
		accumulator += deltaTime;


		//std::cout << "delta: " << deltaTime << std::endl;
	

		
		while (accumulator >= dt) {
			scale -= 0.001f;
			updates++;
			totalUpdates++;
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{

					goto end;
				}
				else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION) {
					game->input(&event);
				}
			}


			game->update(dt, totalUpdates, curFPS);
			t += dt;
			accumulator -= dt;
			// process input with every update
		}


		float alpha = accumulator / dt;
		glPushMatrix();
		//glScalef(scale, scale, 1.0f);
	
	
		
		// render here

		//glViewport(0, 0, game->getWidth(), game->getWidth());
		glClearColor(1.0f, 1.0f, 1.0f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		//glEnable(GL_BLEND);

		game->render(alpha);

		

		frames++;

		glPopMatrix();

		SDL_GL_SwapWindow(game->window);
		//SDL_RenderPresent(game->getRenderer());
	}

end:
	glDeleteBuffers(1, &pbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &render_buf);
	SDL_DestroyWindow(game->window);
	Mix_Quit();
    IMG_Quit();
	SDL_Quit();

	return 0;
}