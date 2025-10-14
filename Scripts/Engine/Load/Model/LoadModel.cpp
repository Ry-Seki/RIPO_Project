/*
 *	@file	LoadModel.cpp
 *	@author	Seki
 */

#include "LoadModel.h"

/*
 *	“Ç‚İ‚İˆ—
 */
void LoadModel::Load() {
	// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	modelHandle = MV1LoadModel(filePath.c_str());
	if (modelHandle == -1) return;
	// “Ç‚İ‚İŠ®—¹
	isLoaded = true;
}
