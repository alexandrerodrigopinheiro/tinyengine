#ifndef TINYENGINE_LTE_GLOBALS_H_
#define TINYENGINE_LTE_GLOBALS_H_

#pragma once

#include <signal.h>
#include <vector>

#include <mutex>
#include <thread>
#include <algorithm>

#include "LTE_Types.h"

#ifndef HAVE_SIGACTION
#error sigaction is not supported
#endif

// sig_atomic_t is defined by C99
static volatile sig_atomic_t LTE_GLOBAL_DONE = 0;
SDL_Surface* LTE_GLOBAL_SCREEN = NULL;
bool LTE_GLOBAL_MUST_SAVE = false;
bool LTE_GLOBAL_MUST_LOAD = false;
std::vector<LTE_ButtonMap> LTE_GLOBAL_BUTTON_EGGBEGIN_ORG;
std::vector<LTE_ButtonMap> LTE_GLOBAL_BUTTON_EGGBEGIN_DST;
LTE_Size LTE_GLOBAL_WINDOW = { 0, 0 };
LTE_Size LTE_GLOBAL_ASPECT = { 0, 0 };
// 1920x1080, 1360x765, 1280x720, 1024x576, 896x504, 640x360

#endif
