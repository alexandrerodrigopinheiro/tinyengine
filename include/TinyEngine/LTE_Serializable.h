#ifndef TINYENGINE_LTE_SERIALIZE_H_
#define TINYENGINE_LTE_SERIALIZE_H_

#pragma once

#include <iostream>

#include "LTE_Utils.h"
#include "LTE_Globals.h"

bool LTE_GetMustSave() { return LTE_GLOBAL_MUST_SAVE; }
bool LTE_GetMustLoad() { return LTE_GLOBAL_MUST_LOAD; }
void LTE_SetMustSave(bool must) { LTE_GLOBAL_MUST_SAVE = must; }
void LTE_SetMustLoad(bool must) { LTE_GLOBAL_MUST_LOAD = must; }
class LTE_Serializable {
public:
	virtual ~LTE_Serializable() {}
	virtual bool Save(std::ofstream& stream) { LTE_Unused(stream); return true; }
	virtual bool Load(std::ifstream& stream) { LTE_Unused(stream); return true; }
};

#endif
