#ifndef TINYENGINE_LTE_ERROR_H_
#define TINYENGINE_LTE_ERROR_H_

#pragma once

#include <cstdlib>

typedef enum {
	exit_success = EXIT_SUCCESS,
	exit_failure = EXIT_FAILURE,
	exit_failed_to_set_video_mode = 2,
	exit_failed_to_load_image,
	exit_failed_to_resize_image,
	exit_failed_to_load_music,
	exit_failed_to_load_sound,
	exit_failed_to_load_font,
	exit_failed_to_render_font,
	exit_failed_to_load_scene
} LTE_ExitCode;

#endif
