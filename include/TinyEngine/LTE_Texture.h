#ifndef TINYENGINE_LTE_TEXTURE_H_
#define TINYENGINE_LTE_TEXTURE_H_

#pragma once

#ifdef USE_OPENGL
#	include "OpenGL/LTE_Texture.h"
#else
#	include "SDL/LTE_Texture.h"
#endif

#include "LTE_Types.h"

class LTE_TextureAnim : protected LTE_Texture {
public:
	LTE_TextureAnim() {}
	LTE_TextureAnim(const LTE_TextureAnim&) = delete;
	LTE_TextureAnim(const std::string& path, const LTE_Size& frameSize, int totalFrames) : LTE_Texture(path), _frameSize(frameSize), _totalFrames(totalFrames) {}
	LTE_TextureAnim(const std::string& path, uint32_t cols, int totalFrames) : LTE_Texture(path), _frameSize(LTE_Size(this->_size.w / cols, this->_size.h / (totalFrames / cols))), _totalFrames(totalFrames) {}
	LTE_TextureAnim(const std::string& path, uint32_t cols, uint32_t rows, int totalFrames) : LTE_Texture(path), _frameSize(LTE_Size(this->_size.w / cols, this->_size.h / rows)), _totalFrames(totalFrames) {}
	virtual ~LTE_TextureAnim() {}
	virtual void LoadAnim(const std::string& path, uint32_t cols, int totalFrames) {
		LTE_Texture::Load(path);
		this->_frameSize = LTE_Size(this->_size.w / cols, this->_size.h / (int)std::ceil((double)totalFrames / (double)cols));
		this->_totalFrames = totalFrames;
	}
	virtual void LoadAnim(const std::string& path, uint32_t cols, uint32_t rows, int totalFrames) {
		LTE_Texture::Load(path);
		this->_frameSize = LTE_Size(this->_size.w / cols, this->_size.h / rows);
		this->_totalFrames = totalFrames;
	}
	virtual void Draw(int frame, int x, int y) const {
		const int row = frame / (this->_size.w / this->_frameSize.w);
		const int col = frame % (this->_size.w / this->_frameSize.w);
		LTE_Texture::Draw(x, y, this->_frameSize.w, this->_frameSize.h, row, col);
	}
	virtual void Draw(int x, int y, int row, int col) const {
		LTE_Texture::Draw(x, y, this->_frameSize.w, this->_frameSize.h, row, col);
	}
	virtual void Draw(int frame, const LTE_Position& pos) const {
		this->Draw(frame, pos.x, pos.y);
	}
	const LTE_Size& GetFrameSize() const { return this->_frameSize; }
	int GetTotalFrames() const { return this->_totalFrames; }
protected:
	LTE_Size _frameSize {0,0};
	int _totalFrames {0};
};

class LTE_TextureStack : protected LTE_Texture {
public:
	LTE_TextureStack() {}
	LTE_TextureStack(const LTE_TextureStack&) = delete;
	LTE_TextureStack(const std::string& path) : LTE_Texture(), _textureStack() { this->_textureStack.push_back(new LTE_Texture(path)); }
	virtual ~LTE_TextureStack() {
		for (std::size_t i = 0; i < this->_textureStack.size(); i++) {
			delete this->_textureStack[i]; this->_textureStack[i] = NULL;
		}
		this->_textureStack.clear();
	}
	virtual void LoadStack(const std::string& path) { this->_textureStack.push_back(new LTE_Texture(path)); }
	virtual void Draw(int stack, int x, int y) const {
		if (stack < 0 || stack >= (int)this->_textureStack.size()) {
			std::cerr << "ERROR: Illegal index to texture stack size " << this->_textureStack.size() << " as " << stack << ", failed" << SDL_GetError() << std::endl;
			exit(0);
		}
		this->_textureStack[stack]->Draw(x, y);
	}
	std::size_t GetSize() { return this->_textureStack.size(); }
protected:
	std::vector<LTE_Texture*> _textureStack;
};

class LTE_BitmapFont : protected LTE_Texture {
public:
	LTE_BitmapFont() {}
	LTE_BitmapFont(const LTE_BitmapFont&) = delete;
	LTE_BitmapFont(const std::string& path, const LTE_Size& frameSize, int totalFrames): LTE_Texture(path), _frameSize(frameSize), _totalFrames(totalFrames), _fontSpacing(frameSize.w), _blankSpacing(frameSize.w), _lineHeight(frameSize.h) {}
	virtual ~LTE_BitmapFont() {}
	virtual void LoadFont(const std::string& path, const LTE_Size& frameSize, int totalFrames) {
		LTE_Texture::Load(path);
		this->_frameSize = frameSize;
		this->_totalFrames = totalFrames;
		this->_fontSpacing = frameSize.w;
		this->_blankSpacing = frameSize.w;
		this->_lineHeight = frameSize.h;
	}
	virtual void Write(LTE_Position position, const std::string& text) const {
		LTE_Position pos = position;
		size_t length = text.length();
		for (size_t i = 0; i < length; i++) {
			if (text[i] == ' ') { // Space
				pos.x += this->_blankSpacing;
			} else if (text[i] == '\n') { // Linebreak
				pos.y += this->_lineHeight;
				int mmm = pos.x;
				pos.x = mmm;
			} else { // Normal ASCII
				int ascii = static_cast<unsigned char>(text[i]);
				int row = ascii / (this->_size.w / this->_frameSize.w);
				int col = ascii % (this->_size.w / this->_frameSize.w);
				LTE_Texture::Draw(pos, this->_frameSize, row, col);
				pos.x += this->_fontSpacing;
			}
		}
	}
	const LTE_Size& GetFrameSize() const { return this->_frameSize; }
	int GetTotalFrames() const { return this->_totalFrames; }
	int GetFontSpacing() const { return this->_fontSpacing; }
	void SetFontSpacing(int value) { this->_fontSpacing = value; }
	void SetBlankSpacing(int value) { this->_blankSpacing = value; }
	void SetLineHeight(int value) { this->_lineHeight = value; }
private:
	LTE_Size _frameSize{ 0, 0 };
	int _totalFrames{ 0 };
	int _fontSpacing{ 0 };
	int _blankSpacing{ 0 };
	int _lineHeight{ 0 };
};

typedef LTE_Texture LTE_Image;
typedef LTE_TextureAnim LTE_Layer;
typedef LTE_TextureStack LTE_Stack;
typedef LTE_BitmapFont LTE_Scribe;

#endif
