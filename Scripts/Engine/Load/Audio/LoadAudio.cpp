/*
 *	@file	LoadAudio.cpp
 *	@author	Seki
 */

#include "LoadAudio.h"
#include "../../../Data/DxLibResourcesManager.h"

/*
 *	@brief	デストラクタ
 */
LoadAudio::~LoadAudio() {
	if(audioHandle != 1) DeleteSoundMem(audioHandle);
}
/*
 *	@brief	読み込み処理
 */
void LoadAudio::Load() {
	audioHandle = LoadSoundMem(filePath.c_str());
	if (audioHandle == -1) {
		assert(false && "Audioの読み込み失敗");
		return;
	}
	isLoaded = true;
	DxLibResourcesManager::GetInstance().AddAudioHandle(audioHandle);
}
