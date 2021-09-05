#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Game.h"

int main(int argc, char** argv) {


	int number = 10 + 5;
	std::cout << number << std::endl;
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
	//game->setDimensions(mode.w / 1.5, mode.h / 1.5);
	//game->setFullscreenDimensions(mode.w, mode.h);
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

	// done once, give us a fresh projection matrix.

	//glDisable(GL_DEPTH_TEST);

	//GLuint pbo;
//	glGenBuffers(1, &pbo);
//	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
//	glBufferData(GL_PIXEL_PACK_BUFFER, game->iw * game->ih * 4, NULL, GL_DYNAMIC_READ);

	GLuint fbo, render_buf, normal_fbo, normal_render_buf;
	GLuint normal_depthrenderbuffer, depthrenderbuffer;
	GLuint texture, normal_texture;

	glGenTextures(1, &normal_texture);
	glBindTexture(GL_TEXTURE_2D, normal_texture);
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, game->iw, game->ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	//glEnable(GL_TEXTURE_2D_MULTISAMPLE);
//	std::cout << glGetError() << std::endl;
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	std::cout << glGetError() << std::endl;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//std::cout << glGetError() << std::endl;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//std::cout << glGetError() << std::endl;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	///	std::cout << glGetError() << std::endl;
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_GENERATE_MIPMAP, GL_TRUE);
			//std::cout << glGetError() << std::endl;;

	}


	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_RGBA, game->iw, game->ih, GL_TRUE);



	glGenRenderbuffers(1, &normal_render_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, normal_render_buf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, game->iw, game->ih);
	glGenRenderbuffers(1, &normal_depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, normal_depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, game->iw, game->ih);
	//std::cout << glGetError() << std::endl;
	
	

	glGenRenderbuffers(1, &render_buf);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buf);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_RGBA8, game->iw, game->ih);
	std::cout << glGetError() << std::endl;
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA8, game->iw, game->ih);

	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH_COMPONENT, game->iw, game->ih);
	std::cout << glGetError() << std::endl;
	
	
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render_buf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glGenFramebuffers(1, &normal_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, normal_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normal_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, normal_render_buf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, normal_depthrenderbuffer);
	std::cout << glGetError() << std::endl;

	std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout << glGetError() << std::endl;
	glBindTexture(GL_TEXTURE_2D, 0);
	std::cout << glGetError() << std::endl;



	SDL_GL_SetSwapInterval(1);
	game->fbo = fbo;
	game->normal_fbo = normal_fbo;
	game->render_buf = render_buf;
//	game->pbo = pbo;
	game->init();

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::cout << glGetError() << std::endl;
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

end:
	//glDeleteBuffers(1, &pbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &normal_fbo);
	glDeleteRenderbuffers(1, &normal_depthrenderbuffer);
	glDeleteRenderbuffers(1, &normal_render_buf);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteRenderbuffers(1, &render_buf);
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &normal_texture);
	SDL_DestroyWindow(game->window);
	Mix_Quit();
    IMG_Quit();
	SDL_Quit();

	delete game;

	return 0;
}