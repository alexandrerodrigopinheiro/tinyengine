#ifndef TINYENGINE_LTE_RENDERGL_H_
#define TINYENGINE_LTE_RENDERGL_H_

#pragma once

#include "LTE_Common.h"
#include "LTE_Font.h"
#include "../Abstract/LTE_Common.h"
#include "../LTE_Types.h"
#include "../LTE_Globals.h"
#include "../LTE_Utils.h"

void LTE_DrawFPS(LTE_Font& font, int fps, const LTE_Position& pos, const LTE_Color& color)
{
	static char buffer[12];
	static int last_fps = -1;
	static SDL_Surface *texture = NULL;
	static SDL_Rect src = { 0, 0, 100, 20 };
	static SDL_Rect dst = { 0, 0, 0, 0 };
	LTE_Position position = LTE_Fluid(pos);
	dst.x = position.x;
	dst.y = position.y;
	if (fps != last_fps) {
		last_fps = fps;
		snprintf(buffer, sizeof(buffer), "%d", fps);
		if (texture != NULL) {
			SDL_FreeSurface(texture);
		}
		SDL_Color sdlColor = { color.r, color.g, color.b, color.a };
		texture = TTF_RenderText_Blended(font.GetNative(), buffer, sdlColor);
	}
	SDL_BlitSurface(texture, &src, LTE_GLOBAL_SCREEN, &dst);
}

void LTE_ScreenClear() { SDL_FillRect(LTE_GLOBAL_SCREEN, NULL, SDL_MapRGB(LTE_GLOBAL_SCREEN->format, 0, 0, 0)); }

void LTE_ScreenSwap() { SDL_Flip(LTE_GLOBAL_SCREEN); }

#endif
