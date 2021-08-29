#ifndef TINYENGINE_LTE_TEXTURESDL_H_
#define TINYENGINE_LTE_TEXTURESDL_H_

#pragma once

#include "../Abstract/LTE_ITexture.h"
#include "../LTE_Error.h"
#include "../Abstract/LTE_Common.h"
#include "../LTE_Types.h"
#include <iostream>
#include <string>
#include "../LTE_Common.h"
#include "../LTE_Utils.h"
#include "../LTE_Globals.h"

class LTE_TextureSDL : public LTE_ITexture<SDL_Surface *> {
public:
	LTE_TextureSDL() {}
	LTE_TextureSDL(const LTE_TextureSDL&) = delete;
	LTE_TextureSDL(const std::string& path) { Load(path); }
	virtual ~LTE_TextureSDL() { if (this->_texture != NULL) { SDL_FreeSurface(this->_texture); }}
	void Load(const std::string& path) {
		std::cout << "Loading image '" << path << "', ";
		SDL_Surface *image = IMG_Load(path.c_str());
		SDL_Surface *optimized_image = NULL;
		if (image != NULL) {
			optimized_image = image->format->BytesPerPixel == 4 ? SDL_DisplayFormatAlpha(image) : SDL_DisplayFormat(image);
			SDL_FreeSurface(image);
		}
		if (optimized_image == NULL) {
			std::cout << "failed. -- " << IMG_GetError() << std::endl;
			exit(exit_failed_to_load_image);
		}
		std::cout << "OK. -- " << static_cast<int>(optimized_image->format->BitsPerPixel) << " bpp" << std::endl;
		this->_size.w = optimized_image->w;
		this->_size.h = optimized_image->h;
		this->_texture = optimized_image;
	}
	virtual void Draw(const LTE_Position& pos, const LTE_Size& size, int row, int col) const {
		LTE_Position position = LTE_Fluid(pos.x, pos.y);
		SDL_Rect src, dst;
		src.x = size.w * col;
		src.y = size.h * row;
		src.w = size.w;
		src.h = size.h;
		dst.x = position.x;
		dst.y = position.y;
		SDL_BlitSurface(_texture, &src, LTE_GLOBAL_SCREEN, &dst);
	}
	virtual void Draw(int x, int y, uint32_t w, uint32_t h, int row, int col) const {
		LTE_Position position = LTE_Fluid(x, y);
		SDL_Rect src, dst;
		src.x = w * col;
		src.y = h * row;
		src.w = w;
		src.h = h;
		dst.x = position.x;
		dst.y = position.y;
		SDL_BlitSurface(_texture, &src, LTE_GLOBAL_SCREEN, &dst);
	}
	virtual void Draw(int x, int y) const {
		SDL_Rect src, dst;
		LTE_Position position = LTE_Fluid(x, y);
		src.x = 0;
		src.y = 0;
		src.w = this->_size.w;
		src.h = this->_size.h;
		dst.x = position.x;
		dst.y = position.y;
		SDL_BlitSurface(_texture, &src, LTE_GLOBAL_SCREEN, &dst);
	}
	SDL_Surface* GetNative() const { return this->_texture; }
protected:
	SDL_Surface* _texture {NULL};
};

typedef LTE_TextureSDL LTE_Texture;

#endif
