/*
 *	@file	LoadAudio.cpp
 *	@author	Seki
 */

#include "LoadAudio.h"

/*
 *	“Ç‚İ‚İˆ—
 */
void LoadAudio::Load() {
	audioHandle = LoadSoundMem(filePath.c_str());
	if (audioHandle == -1) return;

	isLoaded = true;
}
