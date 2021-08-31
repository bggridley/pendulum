#include "Game.h"
#include "math.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Game::Game() {
}

void Game::init() {
	resources.init();
	font.init("res/font.fnt", resources.font.getTexID());

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
			const float w = getWidth();
			const float h = getHeight();

			/*
			GLsizei nrChannels = 3;
			GLsizei stride = nrChannels * w;
			stride += (stride % 4) ? (4 - stride % 4) : 0;
			GLsizei bufferSize = stride * h;
			std::vector<char> buffer(bufferSize);
			glPixelStorei(GL_PACK_ALIGNMENT, 4);
			glReadBuffer(GL_FRONT);
			glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
			*/

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glDisable(GL_SCISSOR_TEST);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glViewport(0, 0, iw, ih);
			glOrtho(0, iw, ih, 0, -1, 1);
			glEnable(GL_MULTISAMPLE);
			//after drawing
			
			//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render(0.66f);
			std::vector<std::uint8_t> data(iw * ih * 4);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glReadPixels(0, 0, iw, ih, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);




			//std::cout << sizeof & data[0] << std::endl;

			// Return to onscreen rendering:


			//glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
			//glReadPixels(0, 0, iw, ih, GL_RGBA, GL_UNSIGNED_BYTE, 0); // 0 instead of a pointer, it is now an offset in the buffer.




			//DO SOME OTHER STUFF (otherwise this is a waste of your time)
			//glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo); //Might not be necessary...


			GLsizei nrChannels = 4;
			GLsizei stride = nrChannels * iw;
			stride += (stride % 4) ? (4 - stride % 4) : 0;

			//void* pixels = 
			stbi_flip_vertically_on_write(true);
			stbi_write_png("test.png", iw, ih, nrChannels, &data[0], stride);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//std::cout << GL_MAX_SAMPLES << std::endl;

			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glViewport(0, 0, getWidth(), getHeight());
			//glViewport(0, 0, getWidth(), getHeight());
			glOrtho(0, getWidth(), getHeight(), 0, -1, 1);
			//glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			//glUnmapBuffer(pbo);
			//glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			//glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); /* unbind the PBO */

			//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
			/*          ^^^^^^--- You need to bind your PBO for UNPACKING! */
			//glDrawPixels(iw, ih, GL_RGBA, GL_FLOAT, 0);
			//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

			//delete[] pixels;
			//stbi_write_png("test.png", w, h, nrChannels, buffer.data(), stride);
			// Convert to FreeImage format & save to file
			//FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
			//FreeImage_Save(FIF_BMP, image, "C:/test.bmp", 0);

			// Free resources
			//FreeImage_Unload(image);

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

void Game::update(float delta, int updateCount, int fps) {
	this->fps = fps;
	this->updates = updateCount;




	theta1 += delta;

	float num1 = -g * (2 * mass1 + mass2) * sin(theta1);
	float num2 = -mass2 * g * sin(theta1 - 2 * theta2);
	float num3 = -2 * sin(theta1 - theta2) * mass2;
	float num4 = theta2_v * theta2_v * length2 + theta1_v * theta1_v * length1 * cos(theta1 - theta2);
	float den = length1 * (2 * mass1 + mass2 - mass2 * cos(2 * theta1 - 2 * theta2));
	float a1_a = (num1 + num2 + num3 * num4) / den;

	num1 = 2 * sin(theta1 - theta2);
	num2 = (theta1_v * theta1_v * length1 * (mass1 + mass2));
	num3 = g * (mass1 + mass2) * cos(theta1);
	num4 = theta2_v * theta2_v * length2 * mass2 * cos(theta1 - theta2);
	den = length2 * (2 * mass1 + mass2 - mass2 * cos(2 * theta1 - 2 * theta2));
	float a2_a = (num1 * (num2 + num3 + num4)) / den;

	pa = a2_a;

	theta1_v += a1_a * delta;
	theta2_v += a2_a * delta;
	theta1 += theta1_v;
	theta2 += theta2_v;

	int midX = sx;//getWidth() / 2;
	int midY = sy;//getHeight() / 2;

	if (zoomOut) {
		scale -= 0.1 * delta;


	}

	if (zoomIn) {
		scale += 0.1 * delta;


	}

	float startX = sx;//getWidth() / 2; //sx;// getWidth() / 2;
	float startY = sy;// getHeight() / 2; //sy;// getHeight() / 2;
	//std::cout << startX << std::endl;

	float c1[] = { abs(pa * 10), abs(pv * 10), 1 - abs(pa) * 3, 1.0f };
	float c2[] = { .0f, .0f, .0f, 1.0f };

	float x = length1 * sin(theta1);
	float y = length1 * cos(theta1);
	//renderer.drawLine(startX, startY, startX + x, startY + y, c2, c2);


	float x2 = length2 * sin(theta2);
	float y2 = length2 * cos(theta2);



	//renderer.drawLine(startX + x, startY + y, startX + x + x2, startY + y + y2, c2, c2);


	//glPopMatrix();

	//std::cout << pa << ",  " << pv << std::endl;



	//renderer.drawPoint(startX + x + x2, startY + y + y2, c1, c2);
	//glPushMatrix();

	// we know that px and py are never gonna be 0 so....

	if (px != 0.0f && py != 0.0f) {
		


		float perpX = (startY + y + y2 - py);
		float perpY = -(startX + x + x2 - px);


		float length = sqrt(perpX * perpX + perpY * perpY);

		perpX /= length;
		perpY /= length;

		//std::cout << "perp:" << perpX << std::endl;
		pointArray.push_back(startX + x + x2 - perpX * 10);
		pointArray.push_back(startY + y + y2 - perpY * 10);
		pointArray.push_back(((((startX + x + x2) * 1.5) - px / 2)) + perpX * 10);
		pointArray.push_back(((((startY + y + y2) * 1.5) - py / 2)) + perpY * 10);
		pointArray.push_back(c1[0]);
		pointArray.push_back(c1[1]);
		pointArray.push_back(c1[2]);
	}
	else {
		std::cout << "bad" << std::endl;
	}

	px = startX + x + x2;
	py = startY + y + y2;

	pv = theta2_v;
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//std::cout << alpha << std::endl;
	//std::string framespersec = "FPS:" + std::to_string(fps);
	//font.setScale(1.0f);
	//font.setColor(1.0f, 1.0f, 1.0f, 1.0f);
	//font.renderString(0, 0, framespersec);

	renderer.setScale(scale);

	//float c1[] = {  1.0f, 0.0f, 0.0f } ;
	//renderer.drawLine(100, 100, 200, 200, c1, c1);
	//renderer.drawLine(200, 200, 300, 100, c1, c1);
	//renderer.drawCircle(getWidth() / 2, getHeight() / 2, getWidth() / 2, 0);
	// 100 is length


	//glLineWidth(10.0f);
	glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_MULTISAMPLE);
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i < pointArray.size(); i += 7) {



		float c[] = { pointArray[i + 4], pointArray[i + 5], pointArray[i + 6] };
		//if (pointArray.size() > 20) {
		//	renderer.drawBezier(pointArray[i - 15], pointArray[i - 14], pointArray[i - 10], pointArray[i - 9], pointArray[i - 5], pointArray[i - 4], pointArray[i], pointArray[i + 1], c);
		//}
		//float cc[] = { pointArray[i + 2], pointArray[i + 3], pointArray[i + 4] };
		//renderer.drawLine(pointArray[i - 5], pointArray[i - 4], pointArray[i], pointArray[i + 1], c, cc);

		glPointSize(10.0f);
		///renderer.drawPoint(pointArray[i], pointArray[i + 1], c[0], c[1], c[2]);
	//	renderer.drawPoint(pointArray[i + 2], pointArray[i + 3], c[0], c[1], c[2]);
		glColor4f(c[0], c[1], c[2], 0.5f);
		glVertex2f(pointArray[i], pointArray[i + 1]);
		glColor4f(c[0], c[1], c[2], 0.5f);
		glVertex2f(pointArray[i + 2], pointArray[i + 3]);
	}


	
	glEnd();
}

void Game::setDimensions(float width, float height) {
	this->height = height;
	this->width = width;
}

void Game::setFullscreenDimensions(float width, float height) {
	this->fullscreenWidth = width;
	this->fullscreenHeight = height;
}

float Game::getWidth() {
	return fullscreen ? fullscreenWidth : width;
}

float Game::getHeight() {
	return fullscreen ? fullscreenHeight : height;
}

float Game::getScale() {
	return scale;
}

BitmapFont Game::getFont() {
	return font;
}

Renderer* Game::getRenderer() {
	return &renderer;
}

Resources Game::getResources() {
	return resources;
}

