#pragma once
#include "Texture.h"
#include "TextureAtlas.h"
#include "glew.h"
#include "sdl.h"
#include "sdl_image.h"
#include "sdl_mixer.h"
#include <string>

class Resources {

public:

	Texture font;
	Resources() {

	}

	void init() {

		font = loadTexture("res/font.png", GL_NEAREST);
	}


private:
	Texture loadTexture(std::string path, GLint param) {
		GLuint texID = 0;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		GLint mode = GL_RGBA;

		int width = loadedSurface->w;
		int height = loadedSurface->h;

		glEnable(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, mode, loadedSurface->w, loadedSurface->h, 0, mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);



		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glGenerateMipmap(GL_TEXTURE_2D);



		glDisable(GL_TEXTURE_2D);

		SDL_FreeSurface(loadedSurface);
		Texture tex;
		tex.init(texID, width, height);
		return tex;
	}

};