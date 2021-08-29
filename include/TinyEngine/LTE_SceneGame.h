#ifndef TINYENGINE_LTE_SCENEGAME_H_
#define TINYENGINE_LTE_SCENEGAME_H_

#pragma once

#include "LTE_Common.h"
#include "LTE_Scene.h"
#include "LTE_SceneManager.h"
#include "LTE_Types.h"

class LTE_SceneGame;
typedef LTE_SceneManager<LTE_SceneMap, LTE_SceneGame> LTE_GameSceneManager;

class LTE_SceneGame : public LTE_Scene<LTE_GameSceneManager> {
public:
	LTE_SceneGame(LTE_GameSceneManager& manager) : LTE_Scene(manager), _pressedInvalid(false), _releasedInvalid(false), _pressedButton(BUTTON_MAP_INVALID), _releasedButton(BUTTON_MAP_INVALID) {}
	virtual ~LTE_SceneGame() {}
	virtual operator std::string() const = 0;
	virtual void Appear() {
		LTE_Scene<LTE_GameSceneManager>::Appear();
		this->Clear();
	}
	virtual void Disappear() { LTE_Scene<LTE_GameSceneManager>::Disappear(); }
	virtual void Event(const SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			bool found = false;
			for (auto key_entry : LTE_KEYMAP) {
				if (event.key.keysym.sym == key_entry.sym) {
					found = true;
					std::cout << std::string(*this) << " - Key pressed: " << key_entry.p_chName << std::endl;
					break;
				}
			}
			if (!found) {
				std::cout << std::string(*this) << " - Key pressed: UNKNOWN" << std::endl;
			}
		}
	}
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;
	void Clear() {
		this->_pressedInvalid = this->_pressedInvalid  ? false : true;
		this->_releasedInvalid = this->_releasedInvalid ? false : true;
		this->_pressedButton = BUTTON_MAP_INVALID;
		this->_releasedButton = BUTTON_MAP_INVALID;
	}
	bool _pressedInvalid { false };
	bool _releasedInvalid { false };
	LTE_ButtonMap _pressedButton { BUTTON_MAP_INVALID };
	LTE_ButtonMap _releasedButton { BUTTON_MAP_INVALID };
};

#endif /* SceneGame_H_ */
