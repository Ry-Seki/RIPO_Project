/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "AudioManager.h"

/*
 *	‰Šú‰»ˆ—
 */
void AudioManager::Initialize() {
}
/*
 *	ƒ[ƒhÏ‚Ý‚Ìƒf[ƒ^‚ÌÝ’è(ƒR[ƒ‹ƒoƒbƒN)
 */
void AudioManager::SetupData() {
}
/*
 *	XVˆ—
 */
void AudioManager::Update() {
	bgmManager->Update();
	seManager->Update();
}
/*
 *	BGM‚ÌÄ¶ˆ—
 */
void AudioManager::PlayBGM() {
	bgmManager->PlayBGM(bgmVolume);
}
/*
 *	BGM‚Ì’âŽ~ˆ—
 */
void AudioManager::StopBGM() {
	bgmManager->StopBGM();
}
/*
 *	BGM‚Ì•ÏXˆ—
 *  @param[in]	const std::string& setKeyName	•ÏX‚·‚éBGM–¼
 */
void AudioManager::ChangeBGM(const std::string& setKeyName) {
	bgmManager->ChangeBGM(setKeyName);
}
/*
 *	BGMƒnƒ“ƒhƒ‹‚Ì“o˜^
 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éBGM–¼
 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
 */
void AudioManager::RegisterBGMHandle(const std::string& setKeyName, const int setHandle) {
	bgmManager->RegisterBGMHandle(setKeyName, setHandle);
}
/*
 *	SEÄ¶ˆ—
 *  @param[in]	const std::string& setKeyName	Ä¶‚·‚éSE–¼
 */
void AudioManager::PlaySE(const std::string& setKeyName) {
	seManager->PlaySE(setKeyName, seVolume);
}
/*
 *	SEƒnƒ“ƒhƒ‹‚Ì“o˜^
 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éSE–¼
 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
 */
void AudioManager::RegisterSEHandle(const std::string& setKeyName, const int setHandle) {
	seManager->RegisterSEHandle(setKeyName, setHandle);
}
