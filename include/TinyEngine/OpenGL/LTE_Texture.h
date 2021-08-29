#ifndef TINYENGINE_LTE_TEXTURE_H_
#define TINYENGINE_LTE_TEXTURE_H_

#pragma once

#include <iostream>
#include <string>
#include "../Abstract/LTE_ITexture.h"
#include "../LTE_Error.h"
#include "LTE_Common.h"
#include "../LTE_Utils.h"
#include "../LTE_Types.h"

class LTE_TextureGL : public LTE_ITexture<GLuint> {
public:
	LTE_TextureGL() {}
	LTE_TextureGL(const LTE_TextureGL&) = delete;
	LTE_TextureGL(const std::string& path) { Load(path); }
	virtual ~LTE_TextureGL() { glDeleteTextures(1, &this->_unit); }
	virtual void Load(const std::string& path) {
		std::cout << "Loading image '" << path << "', ";
		SDL_Surface *image = IMG_Load(path.c_str());
		if (image == NULL) {
			std::cout << "failed. -- " << IMG_GetError() << std::endl;
			exit(exit_failed_to_load_image);
		}
		std::cout << "OK. -- " << static_cast<int>(image->format->BitsPerPixel) << " bpp" << std::endl;
#if 0
		// Resize image if it's not power of two - Required by some old GPUs.
		if (!LTE_IsPowerOfTwo(image->w) || !LTE_IsPowerOfTwo(image->h)) {
			int width  = nextPowerOfTwo(image->w);
			int height = nextPowerOfTwo(image->h);
			std::cout << "WARNING: Texture width/height is not power of two. Scaling to " << width << "x" << height << std::endl;
			SDL_Surface *resized_image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
			if (resized_image == NULL) {
				std::cerr << "ERROR: Failed to resize image: " << SDL_GetError() << std::endl;
				exit(exit_failed_to_resize_image);
			}
			SDL_BlitSurface(image, NULL, resized_image, NULL);
			//SDL_Flip(resized_image);
			SDL_FreeSurface(image);
			image = resized_image;
		}
#endif
		LTE_GL_CHECK(glGenTextures(1, &this->_unit));
		LTE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->_unit));
		GLint internal_format = 0;
		GLint format = 0;
		switch (image->format->BytesPerPixel) {
			case 1:
				internal_format = GL_LUMINANCE;
				format = GL_RED;
				break;
			case 2:
				internal_format = GL_LUMINANCE_ALPHA;
				format = GL_RG;
				break;
			case 3:
			internal_format = GL_RGB;
#ifdef __APPLE__
				format = GL_BGR;
#else
				format = GL_RGB;
#endif
				break;
			case 4:
				internal_format = GL_RGBA;
#ifdef __APPLE__
				format = GL_BGRA;
#else
				format = GL_RGBA;
#endif
				break;
			default:
				std::cerr << "Unknown image format." << std::endl;
				exit(exit_failed_to_load_image);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, image->w % 4 == 0 ? 4 : 1);
		LTE_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels));
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		this->_size.w = image->w;
		this->_size.h = image->h;
		SDL_FreeSurface(image);
	}
	virtual void Draw(const LTE_Position& pos, const LTE_Size& size, int row, int col) const {
		LTE_Position position = LTE_Fluid(pos);
		float x = position.x;
		float y = position.y;
		float w = size.w;
		float h = size.h;
		float tx = (float) (col * size.w) / (float) this->_size.w;
		float ty = (float) (row * size.h) / (float) this->_size.h;
		float tw = (float) ((col + 1) * size.w) / (float) this->_size.w;
		float th = (float) ((row + 1) * size.h) / (float) this->_size.h;
		glEnable(GL_TEXTURE_2D);
		LTE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->_unit));
		glBegin(GL_QUADS);
			glTexCoord2d(tx, ty); glVertex2f(x, LTE_Invert(y));
			glTexCoord2d(tw, ty); glVertex2f(x + w, LTE_Invert(y));
			glTexCoord2d(tw, th); glVertex2f(x + w, LTE_Invert(y + h));
			glTexCoord2d(tx, th); glVertex2f(x, LTE_Invert(y + h));
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	virtual void Draw(const LTE_Position& pos) const {
		LTE_Position position = LTE_Fluid(pos);
		float x = position.x;
		float y = position.y;
		float w = this->_size.w;
		float h = this->_size.h;
		glEnable(GL_TEXTURE_2D);
		LTE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->_unit));
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(x, LTE_Invert(y));
			glTexCoord2d(1, 0); glVertex2f(x + w, LTE_Invert(y));
			glTexCoord2d(1, 1); glVertex2f(x + w, LTE_Invert(y + h));
			glTexCoord2d(0, 1); glVertex2f(x, LTE_Invert(y + h));
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	virtual void Draw(int x, int y, uint32_t w, uint32_t h, int row, int col) const {
		LTE_Position position = LTE_Fluid(x, y);
		float xf = position.x;
		float yf = position.y;
		float fw = w;
		float fh = h;
		float tx = (float) (col * fw) / (float) this->_size.w;
		float ty = (float) (row * fh) / (float) this->_size.h;
		float tw = (float) ((col + 1) * fw) / (float) this->_size.w;
		float th = (float) ((row + 1) * fh) / (float) this->_size.h;
		glEnable(GL_TEXTURE_2D);
		LTE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->_unit));
		glBegin(GL_QUADS);
			glTexCoord2d(tx, ty); glVertex2f(x, LTE_Invert(y));
			glTexCoord2d(tw, ty); glVertex2f(x + w, LTE_Invert(y));
			glTexCoord2d(tw, th); glVertex2f(x + w, LTE_Invert(y + h));
			glTexCoord2d(tx, th); glVertex2f(x, LTE_Invert(y + h));
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	virtual void Draw(int x, int y) const {
		LTE_Position position = LTE_Fluid(x, y);
		float xf = position.x;
		float yf = position.y;
		float w = this->_size.w;
		float h = this->_size.h;
		glEnable(GL_TEXTURE_2D);
		LTE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->_unit));
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2f(xf, LTE_Invert(yf));
			glTexCoord2d(1, 0); glVertex2f(xf + w, LTE_Invert(yf));
			glTexCoord2d(1, 1); glVertex2f(xf + w, LTE_Invert(yf + h));
			glTexCoord2d(0, 1); glVertex2f(xf, LTE_Invert(yf + h));
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	GLuint GetNative() const { return this->_unit; }
protected:
	GLuint _unit {0};
};

typedef LTE_TextureGL LTE_Texture;

#endif
