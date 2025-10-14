/*
 *	@file	LoadSprite.cpp
 *	@author	Seki
 */

#include "LoadSprite.h"

/*
 *	“Ç‚İ‚İˆ—
 */
void LoadSprite::Load() {
	// ‰æ‘œ‚Ì“Ç‚İ‚İ
	graphHandle = LoadGraph(filePath.c_str());
	if (graphHandle == -1) return;

	isLoaded = true;
}
