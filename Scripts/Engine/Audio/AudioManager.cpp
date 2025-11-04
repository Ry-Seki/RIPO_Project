/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "AudioManager.h"

/*
 *	初期化処理
 */
void AudioManager::Initialize() {
}
/*
 *	ロード済みのデータの設定(コールバック)
 */
void AudioManager::Setup() {
}
/*
 *	更新処理
 */
void AudioManager::Update() {
	bgmManager->Update();
	seManager->Update();
}
/*
 *	BGMの再生処理
 */
void AudioManager::PlayBGM() {
	bgmManager->PlayBGM(bgmVolume);
}
/*
 *	BGMの停止処理
 */
void AudioManager::StopBGM() {
	bgmManager->StopBGM();
}
/*
 *	BGMの変更処理
 *  @param[in]	const std::string& setKeyName	変更するBGM名
 */
void AudioManager::ChangeBGM(const std::string& setKeyName) {
	bgmManager->ChangeBGM(setKeyName);
}
