#include "TinyEngine/OpenGL/LTE_Render.h"
#include "TinyEngine/OpenGL/LTE_Common.h"
#include "TinyEngine/OpenGL/LTE_Font.h"

void LTE_DrawFPS(LTE_Font& font, int fps, const LTE_Position& pos, const LTE_Color& color)
{
	static char buffer[12];
	static int last_fps = -1;
	if (fps != last_fps) {
		last_fps = fps;
		snprintf(buffer, sizeof(buffer), "%d", fps);
	}
	font.Draw(buffer, pos, color);
}

void LTE_ScreenClear() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushAttrib(GL_CURRENT_BIT);
}

void LTE_ScreenSwap() {
	glPopAttrib();
	SDL_GL_SwapBuffers();
}
