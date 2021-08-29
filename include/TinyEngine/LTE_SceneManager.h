#ifndef TINYENGINE_LTE_SCENEMANAGER_H_
#define TINYENGINE_LTE_SCENEMANAGER_H_

#pragma once

#include <deque>
#include <vector>

#include "LTE_Scene.h"
#include "LTE_Types.h"

template <class scene_enum_type, class scene_type>
class LTE_SceneManager {
public:
	typedef int LTE_SceneIndexType;
	typedef scene_enum_type LTE_SceneEnumType;
	typedef LTE_SceneManager<LTE_SceneEnumType, scene_type> LTE_ThisType;
	typedef scene_type LTE_SceneType;
	void Clear() {
		if (!_sceneIndexStack.empty()) {
			_sceneIndexStack.clear();
		}
	}
	void NextScene(LTE_SceneIndexType sceneIndex) {
		if (this->_sceneIndexStack.size() > 1) { this->PopScene(); }
		this->PushScene(sceneIndex);
	}
	void PushScene(LTE_SceneIndexType sceneIndex) {
		if (this->_currentSceneIndex != scene_invalid) {
			this->CurrentScene()->Clear();
			this->CurrentScene()->Disappear();
		}
		// Push one scene
		this->_sceneIndexStack.push_back(sceneIndex);
		this->_currentSceneIndex = sceneIndex;
		std::clog << "Pushing scene " << std::string(*CurrentScene()) << std::endl;
		this->CurrentScene()->Appear();
		this->Dump();
	}
	void PopScene() {
		if (_sceneIndexStack.empty()) {
			std::cerr << "ERROR: Illegal call to popScene. The stack is empty!" << std::endl;
			return;
		}
		if (_currentSceneIndex != scene_invalid) {
			this->CurrentScene()->Clear();
			this->CurrentScene()->Disappear();
		}
		// Pop the last scene
		LTE_SceneIndexType poppingSceneIndex = this->_sceneIndexStack.back();
		this->_sceneIndexStack.pop_back();
		// Get the new last scene
		this->_currentSceneIndex = this->_sceneIndexStack.back();
		std::clog << "Popping scene " << std::string(*this->GetScene(poppingSceneIndex)) << std::endl;
		this->CurrentScene()->Appear();
		this->Dump();
	}
	LTE_SceneIndexType CurrentSceneIndex() const { return this->_currentSceneIndex; }
	LTE_SceneType* CurrentScene() const { return this->GetScene(this->_currentSceneIndex); }
	LTE_SceneType* GetScene(LTE_SceneIndexType sceneIndex) const { return this->_allScenes[sceneIndex]; }
	void SetScenes(std::initializer_list<LTE_SceneType *> list) { for (auto *scene : list) { this->_allScenes.push_back(scene); }}
	const std::vector<LTE_SceneType *>& GetScenes() const { return this->_allScenes; }
	std::vector<LTE_SceneType *>& MutableScenes() { return this->_allScenes; }
	void Dump() {
		std::cout << "The scene stack is:" << std::endl;
		int i = 0;
		for (auto sceneIndex : this->_sceneIndexStack) {
			std::cout << "  [" << i << "] " << std::string(*GetScene(sceneIndex)) << std::endl;
			i++;
		}
	}
protected:
	std::deque<int> _sceneIndexStack; // Should have been a std::stack, but we want to dump().
	int _currentSceneIndex {scene_invalid};
	std::vector<LTE_SceneType *> _allScenes;
	enum {
		scene_invalid = -1
	};
};

#endif
