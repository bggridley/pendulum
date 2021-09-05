#include "Game.h"
#include "math.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string> 

Game::Game() {
}

Game::~Game() {
	std::cout << "yat!" << std::endl;
}

void Game::init() {

	pendulum = new DoublePendulum(sx, sy);
	
	for (int i = 0; i < 100; i++) {
		pendulum->randomize();
		updateMany(60 * 60, 1.f / 60.0f);
		printScreen(i);

		if (i % 10 == 0) {
			pendulum->px = 0.0f;
			pendulum->py = 0.0f;
			pendulum->pa = 0.0f;
			pendulum->pv = 0.0f;
			pendulum->theta1_v = 0.0f;
			pendulum->theta2_v = 0.0f;
		}

	}
}

void Game::printScreen(int i) {
	std::cout << glGetError() << std::endl;
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, iw, ih);
	glOrtho(0, iw, ih, 0, -1, 1);


	std::cout << glGetError() << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	render(0.66f);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	std::cout << glGetError() << std::endl;
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	std::cout << glGetError() << std::endl;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, normal_fbo);
	std::cout << glGetError() << std::endl;
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	std::cout << glGetError() << std::endl;
	glBlitFramebuffer(0, 0, iw, ih, 0, 0, iw, ih, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	std::cout << "yea" << glGetError() << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, normal_fbo);
	std::cout << glGetError() << std::endl;

	std::vector<std::uint8_t> data(iw * ih * 4);
	glReadPixels(0, 0, iw, ih, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	std::cout << glGetError() << std::endl;

	GLsizei nrChannels = 4;
	GLsizei stride = nrChannels * iw;
	stride += (stride % 4) ? (4 - stride % 4) : 0;

	//void* pixels = 

	std::string filepath = "output/";
	std::string xt = ".png";
	std::string final = filepath + std::to_string(i) + xt;
	stbi_flip_vertically_on_write(true);
	stbi_write_png(final.c_str(), iw, ih, nrChannels, &data[0], stride);

	data.clear();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//std::cout << GL_MAX_SAMPLES << std::endl;

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, getWidth(), getHeight());
	
	glOrtho(0, getWidth(), getHeight(), 0, -1, 1);

	xt = ".txt";
	final = filepath + std::to_string(i) + xt;
	std::ofstream out(final.c_str());

	std::cout << final.c_str() << std::endl;

	out << pendulum->stheta1 << std::endl;
	out << pendulum->stheta2 << std::endl;
	out << pendulum->mass1 << std::endl;
	out << pendulum->mass2 << std::endl;
	out << pendulum->length1 << std::endl;
	out << pendulum->length2 << std::endl;
	out << pendulum->g << std::endl;
	out << pendulum->linewidth;

	out.close();
}

void Game::input(SDL_Event* event) {
	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_EQUALS:
			zoomIn = true;
			break;
		case SDLK_MINUS:
			zoomOut = true;
			break;
		}
	}

	if (event->type == SDL_KEYUP) {
		switch (event->key.keysym.sym) {
		case SDLK_v:
			vsync = !vsync;
			SDL_GL_SetSwapInterval(vsync);
			break;
		case SDLK_f:
			fullscreen = !fullscreen;
			if (fullscreen) {
				SDL_SetWindowSize(window, fullscreenWidth, fullscreenHeight);
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			}
			else {
				SDL_SetWindowSize(window, width, height);
				SDL_SetWindowFullscreen(window, 0);
			}

			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glViewport(0, 0, getWidth(), getHeight());
			//glViewport(0, 0, getWidth(), getHeight());
			glOrtho(0, getWidth(), getHeight(), 0, -1, 1);

			break;
		case SDLK_EQUALS:
			zoomIn = false;
			break;
		case SDLK_MINUS:
			zoomOut = false;
			break;
		case SDLK_9:
			
			

			break;

		}
	}

	if (event->type == SDL_MOUSEBUTTONDOWN) {
		mousePressed = true;

		//	std::cout << tileX << ", " << tileY << std::endl;

		//	std::cout << mouseGameX
	}

	if (event->type == SDL_MOUSEMOTION) {
		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		this->mouseX = mouseX;
		this->mouseY = mouseY;
	}

	if (event->type == SDL_MOUSEBUTTONUP) {
		mousePressed = false;

	}

}

//void game::updateLogic(float delta) {

//}/

void Game::updateLogic(float delta, int updateCount) {

	if (zoomOut) {
		scale -= 0.1 * delta;


	}

	if (zoomIn) {
		scale += 0.1 * delta;


	}


	pendulum->update(delta, updateCount > 60);



}

void Game::update(float delta, int updateCount, int fps) {
	this->fps = fps;
	this->updates = updateCount;



	//theta1 += delta;
	updateLogic(delta, updateCount);
}

void Game::updateMany(int updates, float deltaTime) {
	updatem = true;
	// should probably reset all of the shit, since we want to start from the very beginning.. oh well.
	for (int i = 0; i < updates; i++) {
		//this->updates = i;
		updateLogic(deltaTime, i);
	}
}

void Game::drawSquiggle(float x, float y, float width, float height) {



	for (int i = 0; i < 1000; i++) {
		float xx = std::cos(i * 3.14 / 180);
		float yy = std::sin(i * 3.14 / 180);

		//dstd::cout << y << std::endl;
		//renderer.drawPoint(x + (xx * (width / scale / 2)), y + ((yy * height / scale / 2)), 1.0f, 1.0f, 1.0f);
	}
}



void Game::render(float alpha) {

	//std::cout << "a" << glGetError() << std::endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//std::cout << alpha << std::endl;
	//std::string framespersec = "FPS:" + std::to_string(fps);
	//font.setScale(1.0f);
	//font.setColor(1.0f, 1.0f, 1.0f, 1.0f);
	//font.renderString(0, 0, framespersec);

	//renderer.setScale(scale);

	pendulum->render(alpha);

	//float c1[] = {  1.0f, 0.0f, 0.0f } ;
	//renderer.drawLine(100, 100, 200, 200, c1, c1);
	//renderer.drawLine(200, 200, 300, 100, c1, c1);
	//renderer.drawCircle(getWidth() / 2, getHeight() / 2, getWidth() / 2, 0);
	// 100 is length


	//glLineWidth(10.0f);
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_MULTISAMPLE);

}