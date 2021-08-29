#ifndef TINYENGINE_LTE_TYPES_H_
#define TINYENGINE_LTE_TYPES_H_

#pragma once

#include <cstdint>

#include <SDL/SDL_keyboard.h>

#define LTE_STR(v) # v

typedef struct LTE_Rect {
	int x, y;
	uint32_t w, h;
	LTE_Rect() : x(0),y(0),w(0),h(0) {}
	LTE_Rect(const LTE_Rect& obj) : x(obj.x),y(obj.y),w(obj.w),h(obj.h) {}
	LTE_Rect(int X, int Y, uint32_t W, uint32_t H) : x(X),y(Y),w(W),h(H) {}
	void Set(int X, int Y, uint32_t W, uint32_t H) { x=X; y=Y; w=W; h=H; }
	void Fill(int arg) { x=arg; y=arg; w=arg; h=arg; }
	void Copy(const LTE_Rect& obj) { x=obj.x; y=obj.y; w=obj.w; h=obj.h; }
	void Clear() { x=0; y=0; w=0; h=0; }
	bool Empty() { return x==0 && y==0 && w==0 && h==0; }
} LTE_Rect;

typedef struct LTE_Size {
	uint32_t w, h;
	LTE_Size() : w(0),h(0) {}
	LTE_Size(const LTE_Size& obj) : w(obj.w),h(obj.h) {}
	LTE_Size(uint32_t W, uint32_t H) : w(W),h(H) {}
	void Set(uint32_t W, uint32_t H) { w=W; h=H; }
	void Fill(int arg) { w=arg; h=arg; }
	void Copy(const LTE_Size& obj) { w=obj.w; h=obj.h; }
	void Clear() { w=0; h=0; }
	bool Empty() { return w==0 && h==0; }
} LTE_Size;

typedef struct LTE_Color {
	uint8_t r, g, b, a;
	LTE_Color() : r(0),g(0),b(0),a(0) {}
	LTE_Color(const LTE_Color& obj) : r(obj.r),g(obj.g),b(obj.b),a(obj.a) {}
	LTE_Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : r(R),g(G),b(B),a(A) {}
	void Set(uint8_t R, uint8_t G, uint8_t B, uint8_t A) { r=R; g=G; b=B; a=A; }
	void Fill(uint8_t arg) { r=arg; g=arg; b=arg; a=arg; }
	void Copy(const LTE_Color& obj) { r=obj.r; g=obj.g; b=obj.b; a=obj.a; }
	void Clear() { r=0; g=0; b=0; a=0; }
	bool Empty() { return r==0 && g==0 && b==0 && a==0; }
} LTE_Color;

typedef struct LTE_Position {
	int x, y;
	LTE_Position() : x(0),y(0) {}
	LTE_Position(const LTE_Position& obj) : x(obj.x),y(obj.y) {}
	LTE_Position(int X, int Y) : x(X),y(Y) {}
	void Set(int X, int Y) { x=X; y=Y; }
	void Fill(int arg) { x=arg; y=arg; }
	void Copy(const LTE_Position& obj) { x=obj.x; y=obj.y; }
	void Clear() { x=0; y=0; }
	bool Empty() { return x==0 && y==0; }
} LTE_Position;

typedef struct LTE_Range {
	int begin;
	int end;
	LTE_Range() : begin(0),end(0) {}
	LTE_Range(const LTE_Range& obj) : begin(obj.begin),end(obj.end) {}
	LTE_Range(int Begin, int End) : begin(Begin),end(End) {}
	void Set(int Begin, int End) { begin=Begin; end=End; }
	void Fill(int arg) { begin=arg; end=arg; }
	void Copy(const LTE_Range& obj) { begin=obj.begin; end=obj.end; }
	void Clear() { begin=0; end=0; }
	bool Empty() { return begin==0 && end==0; }
} LTE_Range;

typedef struct LTE_KeyMap {
	SDLKey sym;
	const char *p_chName;
} LTE_KeyMap;

typedef enum LTE_FontAlign {
	FONT_ALIGN_RIGHT = 0,
	FONT_ALIGN_LEFT,
	FONT_ALIGN_CENTER,
//	FONT_ALIGN_JUSTIFY
} LTE_FontAlign;

typedef enum LTE_FontStyle {
	FONTTTF_STYLE_NORMAL = 0,
	FONT_STYLE_BOLD,
	FONT_STYLE_ITALIC,
	FONT_STYLE_UNDERLINE,
	FONT_STYLE_STRIKETHROUGH
} LTE_FontStyle;

typedef enum LTE_SceneMap
{
	SCENE_MAP_INVALID = -1,
	SCENE_MAP_BOARD,
	SCENE_MAP_BONUS,
	SCENE_MAP_DOWBLE,
	SCENE_MAP_HELP,
	SCENE_MAP_READ,
	SCENE_MAP_RECORDS,
	SCENE_MAP_PASS,
	SCENE_MAP_SET
} LTE_SceneMap;

typedef enum LTE_ButtonMap
{
	BUTTON_MAP_INVALID = 0,
	BUTTON_MAP_EXIT,
	BUTTON_MAP_PLAY,
	BUTTON_MAP_LINE,
	BUTTON_MAP_BET,
	BUTTON_MAP_COIN,
	BUTTON_MAP_PASS,
	BUTTON_MAP_READ,
	BUTTON_MAP_HELP,
} LTE_ButtonMap;

#if DEBUG
constexpr const LTE_KeyMap LTE_KEYMAP[] = {
	{ (SDLKey) 0,  LTE_STR(BUTTON_MAP_INVALID) },
	{ SDLK_ESCAPE, LTE_STR(BUTTON_MAP_EXIT)    },
	{ SDLK_p,      LTE_STR(BUTTON_MAP_PLAY)    },
	{ SDLK_l,      LTE_STR(BUTTON_MAP_LINE)    },
	{ SDLK_b,      LTE_STR(BUTTON_MAP_BET )    },
	{ SDLK_c,      LTE_STR(BUTTON_MAP_COIN)    },
	{ SDLK_k,      LTE_STR(BUTTON_MAP_PASS)    },
	{ SDLK_r,      LTE_STR(BUTTON_MAP_READ)    },
	{ SDLK_h,      LTE_STR(BUTTON_MAP_HELP)    },
};
#else
constexpr const LTE_KeyMap LTE_KEYMAP[] = {
	{ (SDLKey) 0,  LTE_STR(BUTTON_MAP_INVALID) },
	{ SDLK_ESCAPE, LTE_STR(BUTTON_MAP_EXIT)    },
	{ SDLK_b,      LTE_STR(BUTTON_MAP_PLAY)    },
	{ SDLK_n,      LTE_STR(BUTTON_MAP_LINE)    },
	{ SDLK_g,      LTE_STR(BUTTON_MAP_BET )    },
	{ SDLK_p,      LTE_STR(BUTTON_MAP_COIN)    },
	{ SDLK_r,      LTE_STR(BUTTON_MAP_PASS)    },
	{ SDLK_4,      LTE_STR(BUTTON_MAP_READ)    },
	{ SDLK_t,      LTE_STR(BUTTON_MAP_HELP)    },
};
#endif

const LTE_KeyMap& LTE_GetKeyMap(const LTE_ButtonMap& buttonMap) {
	switch (buttonMap) {
		default: break;
		case BUTTON_MAP_EXIT: { return LTE_KEYMAP[BUTTON_MAP_EXIT]; }
		case BUTTON_MAP_PLAY: { return LTE_KEYMAP[BUTTON_MAP_PLAY]; }
		case BUTTON_MAP_LINE: { return LTE_KEYMAP[BUTTON_MAP_LINE]; }
		case BUTTON_MAP_BET : { return LTE_KEYMAP[BUTTON_MAP_BET ]; }
		case BUTTON_MAP_COIN: { return LTE_KEYMAP[BUTTON_MAP_COIN]; }
		case BUTTON_MAP_PASS: { return LTE_KEYMAP[BUTTON_MAP_PASS]; }
		case BUTTON_MAP_READ: { return LTE_KEYMAP[BUTTON_MAP_READ]; }
		case BUTTON_MAP_HELP: { return LTE_KEYMAP[BUTTON_MAP_HELP]; }
	}
	return LTE_KEYMAP[BUTTON_MAP_INVALID];
}

#endif
