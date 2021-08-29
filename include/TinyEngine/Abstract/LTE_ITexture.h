#ifndef TINYENGINE_LTE_ITEXTURE_H_
#define TINYENGINE_LTE_ITEXTURE_H_

#pragma once

#include <string>
#include <SDL/SDL.h>
#include "../LTE_Types.h"
#include "../LTE_Utils.h"

template <class T>
class LTE_ITexture {
public:
	LTE_ITexture() {}
	LTE_ITexture(const LTE_ITexture&) = delete;
	LTE_ITexture(const std::string& path) { LTE_Unused(path); }
	virtual ~LTE_ITexture() {}
	virtual void Load(const std::string& path) = 0;
	virtual void Draw(int x, int y, uint32_t w, uint32_t h, int row, int col) const = 0;
	virtual void Draw(int x, int y) const = 0;
	virtual T GetNative() const = 0;
	const LTE_Size& GetSize() const { return this->_size; }
protected:
	LTE_Size _size {0,0};
};

#endif
