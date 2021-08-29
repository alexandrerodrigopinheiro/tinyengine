#ifndef TINYENGINE_LTE_SCENE_H_
#define TINYENGINE_LTE_SCENE_H_

#pragma once

#include <string>
#include <SDL/SDL.h>

#include "LTE_Serializable.h"
#include "LTE_Utils.h"
#include "LTE_Types.h"

template <class scene_enum_type, class scene_type>
class LTE_SceneManager;

template <class scene_manager_type>
class LTE_Scene : public LTE_Serializable {
public:
	typedef scene_manager_type LTE_SceneManagerType;
	LTE_Scene() = delete;
	LTE_Scene(const LTE_Scene&) = delete;
	LTE_Scene(LTE_SceneManagerType& manager) : _manager(manager) {}
	virtual ~LTE_Scene() {}
	// We do NOT use `operator const char *` to avoid implicit conversions.
	virtual operator std::string() const = 0;
	virtual void Appear() { _didAppear = true; }
	virtual void Disappear() { _didAppear = false; }
	virtual bool Load() = 0;
	virtual bool Unload() = 0;
	virtual void Event(const SDL_Event& event) = 0;
	virtual void Update(float tpf) = 0;
	virtual void Render() = 0;
	virtual void Clear() = 0;
protected:
	LTE_SceneManagerType& _manager;
	bool _didAppear {false};
	static bool _interaction_enabled;
};

template <class scene_manager_type>
bool LTE_Scene<scene_manager_type>::_interaction_enabled = false;

#endif
