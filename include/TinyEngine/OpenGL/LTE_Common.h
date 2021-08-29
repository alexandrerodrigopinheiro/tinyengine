#ifndef TINYENGINE_LTE_COMMON_H_
#define TINYENGINE_LTE_COMMON_H_

#pragma once

#include <cstdio>

#if defined(__APPLE__)
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#else
#	include <GL/gl.h>
#	include <GL/glu.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

#define LTE_GL_CHECK(x) \
	do { \
		x; \
		GLenum err = glGetError(); \
		while (err != GL_NO_ERROR) { \
			if (err != GL_NO_ERROR) \
				fprintf(stderr, "ERROR: %s:%d: glGetError() returned %s for call '%s'\n", \
					__FILE__, __LINE__, gluErrorString(err), #x ); \
			err = glGetError(); \
		} \
	} while (0)

#endif
