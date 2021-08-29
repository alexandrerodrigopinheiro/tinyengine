#ifndef TINYENGINE_LTE_IFONT_H_
#define TINYENGINE_LTE_IFONT_H_

#pragma once

#include <string>

#include "../LTE_Types.h"
#include "../LTE_Utils.h"

template <class T>
class LTE_IFont {
public:
	LTE_IFont() {}
	LTE_IFont(const LTE_IFont&) = delete;
	LTE_IFont(const std::string& path, int ptsize) { LTE_Unused(path, ptsize); }
	virtual ~LTE_IFont() {}
	virtual void Load(const std::string& path, int ptsize) = 0;
	virtual void Draw(const char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const = 0;
	virtual void SetStyle(const LTE_FontStyle& style) = 0;
	virtual T GetNative() const = 0;
};

#endif
