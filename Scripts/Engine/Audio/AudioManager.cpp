/*
 *	@file	AudioManager.cpp
 *	@author	Seki
 */

#include "AudioManager.h"

/*
 *	@brief	‰Šú‰»ˆ—
 */
void AudioManager::Initialize() {
	masterVolume = 1.0f;
	bgmVolume = 1.0f;
	seVolume = 1.0f;
}
/*
 *	@brief	XVˆ—
 */
void AudioManager::Update() {
	seManager->Update();
}
/*
 *	@brief	BGM‚ÌÄ¶ˆ—
 */
void AudioManager::PlayBGM() {
	const float volume = masterVolume * bgmVolume;
	bgmManager->PlayBGM(volume);
}
/*
 *	@brief	BGM‚Ì’âŽ~ˆ—
 */
void AudioManager::StopBGM() {
	bgmManager->StopBGM();
}
/*
 *	@brief		BGM‚Ì•ÏXˆ—
 *  @param[in]	const std::string& setKeyName	•ÏX‚·‚éBGM–¼
 */
void AudioManager::ChangeBGM(const std::string& setKeyName) {
	bgmManager->ChangeBGM(setKeyName);
}
/*
 *	@brief		BGMƒnƒ“ƒhƒ‹‚Ì“o˜^
 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éBGM–¼
 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
 */
void AudioManager::RegisterBGMHandle(const std::string& setKeyName, const int setHandle) {
	bgmManager->RegisterBGMHandle(setKeyName, setHandle);
}
/*
 *	@brief		BGM‰¹—Ê•ÏX‚Ì“K‰ž
 */
void AudioManager::ApplyBGMVolume() {
	const float finalVolume = masterVolume * bgmVolume;
	bgmManager->UpdateVolume(finalVolume);
}
/*
 *	@brief		SEÄ¶ˆ—
 *  @param[in]	const std::string& setKeyName	Ä¶‚·‚éSE–¼
 */
void AudioManager::PlaySE(const std::string& setKeyName) {
	const float volume = masterVolume * seVolume;
	seManager->PlaySE(setKeyName, volume);
}
/*
 *	@brief		SEƒnƒ“ƒhƒ‹‚Ì“o˜^
 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éSE–¼
 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
 */
void AudioManager::RegisterSEHandle(const std::string& setKeyName, const int setHandle) {
	seManager->RegisterSEHandle(setKeyName, setHandle);
}
