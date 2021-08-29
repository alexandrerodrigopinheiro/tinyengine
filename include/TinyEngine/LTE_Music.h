#ifndef TINYENGINE_LTE_MUSIC_H_
#define TINYENGINE_LTE_MUSIC_H_

#pragma once

#include <iostream>
#include <string>
#include <SDL/SDL_mixer.h>
#include "LTE_Error.h"

class LTE_Music {
public:
	LTE_Music() : _loop(-1), _play(false), _music(NULL) {}
	LTE_Music(const std::string& path) : _loop(-1), _play(false), _music(NULL) { this->Load(path); }
	LTE_Music(const LTE_Music&) = delete;
	~LTE_Music(){}
	void Release() { Mix_FreeMusic(this->_music); }
	void Load(const std::string& path) {
		std::cout << "Loading music '" << path << "'\n";
		this->_music = Mix_LoadMUS(path.c_str());
		if (this->_music == NULL) {
			std::cerr << "Failed to load music '" << path << "'" << std::endl;
			std::cerr << "Reason: " << Mix_GetError() << std::endl;
			exit(exit_failed_to_load_sound);
		};
	}
	void Play() {
		Mix_PlayMusic(this->_music, this->_loop);
		this->_play = true;
	}
	void Stop() {
		if (this->_play) {
			Mix_PauseMusic();
			this->_play = false;
		}
	}
	void Loop() { this->_loop = this->_loop == 0 ? -1 : this->_loop; }
	bool Played() { return this->_play; } //Mix_PausedMusic() == 0; }
	void SetVolume(uint8_t volume) { Mix_VolumeMusic(volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume); };
protected:
	int _loop { -1 };
	bool _play { false };
	Mix_Music* _music { NULL };
};

#endif
