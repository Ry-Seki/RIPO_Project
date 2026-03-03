/*
 *	@file	LoadEffect.cpp
 *	@author	Seki
 */

#include "LoadEffect.h"

#include <EffekseerForDXLib.h>

/*
 *	@brief		デストラクタ
 */
LoadEffect::~LoadEffect() {
	if(effectHandle != -1) DeleteEffekseerEffect(effectHandle);
}
/*
 *	@brief	読み込み処理
 */
void LoadEffect::Load() {
	effectHandle = LoadEffekseerEffect(filePath.c_str(), magnification);
	// 読み込み失敗
	if (effectHandle == -1) {
		assert(false && "エフェクト読み込み失敗");
		return;
	}
	// 読み込み完了
	isLoaded = true;
}
