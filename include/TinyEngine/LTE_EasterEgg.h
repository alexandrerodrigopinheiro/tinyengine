#ifndef TINYENGINE_LTE_EASTEREGG_H_
#define TINYENGINE_LTE_EASTEREGG_H_

#pragma once

#include "LTE_Globals.h"
#include "LTE_Types.h"

void LTE_EasterEggAttach(const LTE_ButtonMap& button) {
	LTE_GLOBAL_BUTTON_EGGBEGIN_ORG.push_back(button);
}

void LTE_EasterEgg(const LTE_ButtonMap& button, bool enable) {
	if (enable) {
		LTE_GLOBAL_BUTTON_EGGBEGIN_DST.push_back(button);
	}
}

bool LTE_EasterEgg() {
	bool mOut = false;
	if (LTE_GLOBAL_BUTTON_EGGBEGIN_ORG.size() == LTE_GLOBAL_BUTTON_EGGBEGIN_DST.size() && std::equal(LTE_GLOBAL_BUTTON_EGGBEGIN_ORG.begin(), LTE_GLOBAL_BUTTON_EGGBEGIN_ORG.end(), LTE_GLOBAL_BUTTON_EGGBEGIN_DST.begin())) {
		mOut = true;
	}
	LTE_GLOBAL_BUTTON_EGGBEGIN_DST.clear();
	return mOut;
}

#endif /* TINYENGINE_LTE_EASTEREGG_H_ */
