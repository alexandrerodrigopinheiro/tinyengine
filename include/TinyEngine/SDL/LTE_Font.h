#ifndef TINYENGINE_LTE_FONTSDL_H_
#define TINYENGINE_LTE_FONTSDL_H_

#pragma once

#include "../Abstract/LTE_IFont.h"
#include "../LTE_Error.h"
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <string>
#include "LTE_Common.h"
#include "../Abstract/LTE_Common.h"
#include "../LTE_Utils.h"
#include "../LTE_Types.h"
#include "../LTE_Globals.h"

class LTE_FontSDL : public LTE_IFont<TTF_Font *> {
public:
	LTE_FontSDL() : _font(NULL), _stroke(NULL), _colorStroke(), _thickness(0) {}
	LTE_FontSDL(const LTE_FontSDL&) = delete;
	LTE_FontSDL(const std::string& path, int ptsize) : _font(NULL), _stroke(NULL), _colorStroke(), _thickness(0) { this->Load(path, ptsize); }
	LTE_FontSDL(const std::string& path, int ptsize, int thickness, uint8_t r, uint8_t g, uint8_t b, uint8_t a) : _font(NULL), _stroke(NULL), _colorStroke(r, g, b, a), _thickness(0) { this->Load(path, ptsize, thickness, r, g, b, a); }
	virtual ~LTE_FontSDL() {
		if (this->_font != NULL) { TTF_CloseFont(this->_font); }
		if (this->_stroke != NULL) { TTF_CloseFont(this->_stroke); }
	}
	virtual void Load(const std::string& path, int ptsize) {
		std::cout << "Loading font '" << path << "', ";
		this->_font = TTF_OpenFont(path.c_str(), LTE_FluidFont(ptsize));
		if (this->_font == NULL) {
			std::cout << "failed. -- " << TTF_GetError() << std::endl;
			exit(exit_failed_to_load_font);
		}
		std::cout << "OK." << std::endl;
	}
	virtual void Load(const std::string& path, int ptsize, int thickness, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		std::cout << "Loading font '" << path << "', ";
		this->_font = TTF_OpenFont(path.c_str(), LTE_FluidFont(ptsize));
		this->_stroke = TTF_OpenFont(path.c_str(), LTE_FluidFont(ptsize));
		if (this->_font == NULL || this->_stroke == NULL) {
			std::cout << "failed. -- " << TTF_GetError() << std::endl;
			exit(exit_failed_to_load_font);
		}
		this->_thickness = LTE_FluidFont(thickness);
		this->_colorStroke = LTE_Color(r, g, b, a);
		TTF_SetFontOutline(this->_stroke, this->_thickness);
		std::cout << "OK." << std::endl;
	}
	virtual void Draw(const char *text, const LTE_Position pos, const LTE_Color color) const {
		// Do nothing if there is no text.
		if (text[0] == '\0') { return; }
		if (this->_stroke != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_stroke, text, { this->_colorStroke.r, this->_colorStroke.g, this->_colorStroke.b, this->_colorStroke.a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(pos);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x - this->_thickness;
			dst.y = position.y - this->_thickness;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
		if (this->_font != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_font, text, { color.r, color.g, color.b, color.a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(pos);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x;
			dst.y = position.y;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
	}
	virtual void Draw(const char *text, const LTE_Position pos, const LTE_Color color, const LTE_FontAlign& align) const {
		// Do nothing if there is no text.
		if (text[0] == '\0') { return; }
		if (this->_stroke != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_stroke, text, { this->_colorStroke.r, this->_colorStroke.g, this->_colorStroke.b, this->_colorStroke.a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(pos);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x - (align == FONT_ALIGN_RIGHT ? texture->w - this->_thickness : (align == FONT_ALIGN_CENTER ? texture->w / 2: this->_thickness));
			dst.y = position.y - this->_thickness;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
		if (this->_font != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_font, text, { color.r, color.g, color.b, color.a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(pos);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x - (align == FONT_ALIGN_RIGHT ? texture->w - this->_thickness: (align == FONT_ALIGN_CENTER ? (texture->w / 2) + this->_thickness : 0));
			dst.y = position.y;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
	}
	virtual void Draw(const char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
		// Do nothing if there is no text.
		if (text[0] == '\0') { return; }
		if (this->_stroke != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_stroke, text, { this->_colorStroke.r, this->_colorStroke.g, this->_colorStroke.b, this->_colorStroke.a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(x, y);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x - this->_thickness;
			dst.y = position.y - this->_thickness;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
		if (this->_font != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_font, text, { r, g, b, a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(x, y);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x;
			dst.y = position.y;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
	}
	virtual void Draw(const char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const LTE_FontAlign& align) const {
		// Do nothing if there is no text.
		if (text[0] == '\0') { return; }
		if (this->_stroke != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_stroke, text, { this->_colorStroke.r, this->_colorStroke.g, this->_colorStroke.b, this->_colorStroke.a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(x, y);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x - (align == FONT_ALIGN_RIGHT ? texture->w - this->_thickness: (align == FONT_ALIGN_CENTER ? (texture->w / 2) + this->_thickness : this->_thickness));
			dst.y = position.y - this->_thickness;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
		// Do nothing if there is no text.
		if (this->_font != NULL)
		{
			static SDL_Rect src = { 0, 0, 0, 0 };
			static SDL_Rect dst = { 0, 0, 0, 0 };
			SDL_Surface *texture = TTF_RenderText_Blended(this->_font, text, { r, g, b, a });
			if (texture == NULL) {
				std::cout << "Rendering font, failed. -- " << TTF_GetError() << std::endl;
				exit(exit_failed_to_render_font);
			}
			LTE_Position position = LTE_Fluid(x, y);
			src = { 0, 0, 0, 0 };
			dst = { 0, 0, 0, 0 };
			src.w = texture->w;
			src.h = texture->h;
			dst.x = position.x - (align == FONT_ALIGN_RIGHT ? texture->w : (align == FONT_ALIGN_CENTER ? (texture->w / 2) + this->_thickness : 0));
			dst.y = position.y;
			SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
			SDL_FreeSurface(texture);
		}
	}
	virtual void Draw(const char *text, int fX, int fY, uint8_t fR, uint8_t fG, uint8_t fB, uint8_t fA, const LTE_FontAlign& align, int sX, int sY, uint8_t sR, uint8_t sG, uint8_t sB, uint8_t sA) const {
		this->Draw(text, fX + sX, fY + sY, sR, sG, sB, sA, align);
		this->Draw(text, fX, fY, fR, fG, fB, fA, align);
	}
	virtual void SetStyle(const LTE_FontStyle& style) {
		if (this->_font != NULL) {
			TTF_SetFontStyle(this->_font, style);
		}
		if (this->_stroke != NULL) {
			TTF_SetFontStyle(this->_stroke, style);
		}
	}
	TTF_Font *GetNative() const { return this->_font; }
protected:
	TTF_Font *_font {NULL};
	TTF_Font* _stroke { NULL };
	LTE_Color _colorStroke { LTE_Color() };
	int _thickness { 0 };
};

typedef LTE_FontSDL LTE_Font;

#endif
