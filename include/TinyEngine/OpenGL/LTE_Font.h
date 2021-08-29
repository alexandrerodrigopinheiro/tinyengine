#ifndef TINYENGINE_LTE_FONT_H_
#define TINYENGINE_LTE_FONT_H_

#pragma once

#include "../Abstract/LTE_IFont.h"
#include <iostream>
#include <string>
#include "LTE_Common.h"
#include "../LTE_Error.h"
#include "FTGLFontManager.h"

class LTE_FontGL : public LTE_IFont<FTFont *> {
public:
	LTE_FontGL() {}
	LTE_FontGL(const LTE_FontGL&) = delete;
	LTE_FontGL(const std::string& path, int ptsize) { this->Load(path, ptsize); }
	~LTE_FontGL() { /*We do not remove it from FTGLFontManager.*/ }
	virtual void Load(const std::string& path, int ptsize) {
		std::cout << "Loading font '" << path << "', ";
		this->_font = FTGLFontManager::instance().getFont(path.c_str(), ptsize);
		if (this->_font == NULL) {
			std::cout << "failed. -- " << FTGLFontManager::instance().last_error() << std::endl;
			exit(exit_failed_to_load_font);
		}
		std::cout << "OK." << std::endl;
	}
	virtual void Draw(const char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(1, -1, 1); // Invert Y
		glColor4f(r/255.0, g/255.0, b/255.0, a/255.0);
		//x - (align == FONT_ALIGN_RIGHT ? texture->w : (align == FONT_ALIGN_CENTER ? texture->w / 2 : 0))
		LTE_Unused(align);
		this->_font->Render(text, -1, FTPoint(x, -y - this->_font->Ascender() - this->_font->Descender()));
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	virtual void Draw(const char *text, const LTE_Position& pos, const LTE_Color& color) const { this->Draw(text, pos.x, pos.y, color.r, color.g, color.b, color.a); }
	virtual void SetStyle(const LTE_FontStyle& style) {
		if (this->_font != NULL) {
			TTF_SetFontStyle(this->_font, style);
		}
	}
	FTFont *GetNative() const { return this->_font; }
protected:
	FTFont *_font {NULL};
};

typedef LTE_FontGL LTE_Font;

#endif
