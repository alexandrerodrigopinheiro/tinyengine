#ifndef TINYENGINE_LTE_INIT_H_
#define TINYENGINE_LTE_INIT_H_

#pragma once

#include <iostream>
#include <signal.h>
#include "LTE_Common.h"
#include "LTE_Error.h"
#include "LTE_Font.h"
#include "LTE_Sound.h"
#include "OpenGL/FTGLFontManager.h"
#include "LTE_Globals.h"

#ifndef _GNU_SOURCE
#define _POSIX_C_SOURCE 2 // POSIX.1, POSIX.2
#define _SVID_SOURCE // SVID, ISO C, POSIX.1, POSIX.2, X/Open
#define _BSD_SOURCE 1 // 4.3 BSD Unix, ISO C, POSIX.1, POSIX.2
#endif

#if _POSIX_C_SOURCE >= 1 || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#ifndef HAVE_SIGACTION
#define HAVE_SIGACTION // sigaction is part of POSIX.1
#endif
#endif

#ifndef HAVE_SIGACTION
#error sigaction is not supported
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_quit.h>

typedef struct {
	struct {
		uint16_t aspect_w;
		uint16_t aspect_h;
		uint16_t width;
		uint16_t height;
		std::string caption;
	} window;
	struct {
		uint8_t fps;
	} game;
	struct {
		uint32_t frequency;
		uint8_t num_channels;
		uint16_t chunk_size;
	} audio;
	struct {
		struct {
			uint32_t enable_hardware_palette:1;
			uint32_t enable_hardware_surface:1;
			uint32_t enable_fullscreen:1;
			uint32_t enable_multiple_cores:1;
			uint32_t force_video_surface:1;
		} flags;
	} video;
} LTE_Config;

sig_atomic_t LTE_GetDone() { return LTE_GLOBAL_DONE; }

SDL_Surface* LTE_GetScreen() { return LTE_GLOBAL_SCREEN; }

bool LTE_Terminated(void) { return LTE_GLOBAL_DONE == 1; }

void LTE_Terminate() { LTE_GLOBAL_DONE = 1; }

static void LTE_CleanUp(int signal)
{
	printf("Received signal %d\n", signal);
	switch (signal) {
		default: break;
		case SIGINT:
			if (LTE_Terminated()) {
				std::cerr << "Double SIGINT received. Exiting..." << std::endl;
				exit(EXIT_FAILURE);
			} else {
				LTE_Terminate();
			}
			break;
	}
}

static void LTE_InstallSigHandlers(void) {
	struct sigaction sa;
	sa.sa_handler = &LTE_CleanUp;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGHUP, &sa, nullptr);
	sigaction(SIGINT, &sa, nullptr);
	sigaction(SIGTERM, &sa, nullptr);
	sigaction(SIGQUIT, &sa, nullptr);
}

bool LTE_Initialize(const LTE_Config& config) {
	LTE_InstallSigHandlers();
	// TODO: check return values
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
#ifndef USE_OPENGL
	TTF_Init();
#endif
	SDL_ShowCursor(SDL_DISABLE);
	// Disable key repeat
	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_WM_SetCaption(config.window.caption.c_str(), NULL);
	Mix_OpenAudio(config.audio.frequency, MIX_DEFAULT_FORMAT, config.audio.num_channels, config.audio.chunk_size);
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
	if (videoInfo == NULL) {
		std::cout << "WARNING: Unable to gather video informations." << std::endl;
	}
#ifdef USE_OPENGL
	uint32_t videoFlags = SDL_OPENGL;
	// Set the minimum requirements for the OpenGL window
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#else
	uint32_t videoFlags = config.video.flags.enable_hardware_surface || (videoInfo != NULL && videoInfo->hw_available) ? SDL_HWSURFACE | SDL_DOUBLEBUF : SDL_SWSURFACE;
	if (config.video.flags.enable_multiple_cores) {
		videoFlags |= SDL_ASYNCBLIT;
	}
	if (config.video.flags.force_video_surface) {
		videoFlags |= SDL_ANYFORMAT;
	}
	if (config.video.flags.enable_hardware_palette) {
		videoFlags |= SDL_HWPALETTE;
	}
#endif
	if (config.video.flags.enable_fullscreen) {
		videoFlags |= SDL_FULLSCREEN;
	}
	if (videoInfo->hw_available) {
		std::cout << "Total video memory: " << videoInfo->video_mem / 1024 << " MB" << std::endl;
	}
	int width = videoInfo == NULL ? (config.window.width <= 0 ? videoInfo->current_w : config.window.width) : config.window.width;
	int height = videoInfo == NULL ? (config.window.height <= 0 ? videoInfo->current_h : config.window.height) : config.window.height;
	LTE_GLOBAL_WINDOW = LTE_Size(width, height);
	LTE_GLOBAL_ASPECT = LTE_Size(config.window.aspect_w, config.window.aspect_h);
	uint8_t bpp = videoInfo == NULL ? videoInfo->vfmt->BitsPerPixel : 16;
	LTE_GLOBAL_SCREEN = SDL_SetVideoMode(width, height, bpp, videoFlags);
	if (LTE_GLOBAL_SCREEN == NULL) {
		std::cerr << "ERROR: Video initialization failed: " << SDL_GetError() << std::endl;
		exit(exit_failed_to_set_video_mode);
	}
	std::cout << "Video mode: " << LTE_GLOBAL_SCREEN->w << "x" << LTE_GLOBAL_SCREEN->h << " - " << (int)LTE_GLOBAL_SCREEN->format->BitsPerPixel << " bpp" << std::endl;
#ifdef USE_OPENGL
	int maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("GL_MAX_TEXTURE_SIZE = %d\n", maxTextureSize);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, config.window.width, config.window.height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Do not draw alpha bits when they are 0
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_ALPHA_TEST);
#endif
	return true;
}

void LTE_Shutdown() {
#ifdef USE_OPENGL
	FTGLFontManager::instance().cleanup();
#endif
	SDL_FreeSurface(LTE_GLOBAL_SCREEN);
	Mix_CloseAudio();
#ifndef USE_OPENGL
	TTF_Quit();
#endif
	IMG_Quit();
	SDL_Quit();
}

#endif
