/*
 *	@file	LoadSprite.cpp
 *	@author	Seki
 */

#include "LoadSprite.h"
#include "../../../Data/DxLibResourcesManager.h"

#include <DxLib.h>

/*
 *	@brief	デストラクタ
 */
LoadSprite::~LoadSprite() {
	if(graphHandle != -1) DeleteGraph(graphHandle);
}
/*
 *	@brief	読み込み処理
 */
void LoadSprite::Load() {
	// 画像の読み込み
	graphHandle = LoadGraph(filePath.c_str());
	if (graphHandle == -1) {
		assert(false && "画像の読み込み失敗");
		return;
	}

	isLoaded = true;
	DxLibResourcesManager::GetInstance().AddGraphHandle(graphHandle);
}
