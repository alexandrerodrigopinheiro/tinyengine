#ifndef TINYENGINE_LTE_SOUND_H_
#define TINYENGINE_LTE_SOUND_H_

#pragma once

#include <iostream>
#include <string>
#include <SDL/SDL_mixer.h>
#include "LTE_Error.h"

class LTE_Sound {
public:
	LTE_Sound() : _channel(-1), _loop(0), _sound(NULL) {}
	LTE_Sound(const std::string& path) : _channel(-1), _loop(0), _sound(NULL) { this->Load(path); }
	LTE_Sound(const LTE_Sound&) = delete;
	~LTE_Sound(){}
	void Release() { Mix_FreeChunk(this->_sound); }
	void Load(const std::string& path) {
		std::cout << "Loading sound '" << path << "'\n";
		this->_sound = Mix_LoadWAV(path.c_str());
		if (this->_sound == NULL) {
			std::cerr << "Failed to load sound '" << path << "'" << std::endl;
			std::cerr << "Reason: " << Mix_GetError() << std::endl;
			exit(exit_failed_to_load_sound);
		};
	}
	void Play(int channel) { Mix_PlayChannel(channel, this->_sound, this->_loop); }
	void Play() { Mix_PlayChannel(_channel, this->_sound, this->_loop); }
	void Stop() { Mix_HaltChannel(_channel); }
	void Loop() { this->_loop = this->_loop == 0 ? -1 : this->_loop; }
	bool Played() { return Mix_Playing(_channel) == 1; }
	void SetVolume(uint8_t volume) { Mix_VolumeChunk(this->_sound, volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume); };
protected:
	int _channel		{ -1 };
	int _loop			{ 0 };
	Mix_Chunk * _sound	{ NULL };
};

#endif
