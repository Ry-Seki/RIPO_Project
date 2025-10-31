/*
 *	@file	BGMManager.cpp
 *	@author	Seki
 */

#include "BGMManager.h"

/*
 *	‰Šú‰»ˆ—
 */
void BGMManager::Initialize() {
}
/*
 *	ƒ[ƒhÏ‚İ‚Ìƒf[ƒ^‚Ìİ’è(ƒR[ƒ‹ƒoƒbƒN)
 */
void BGMManager::Setup() {
}
/*
 *	XVˆ—
 */
void BGMManager::Update() {
}
/*
 *	•`‰æˆ—
 */
void BGMManager::Render() {
}
/*
 *	BGM‚Ì”jŠüˆ—
 */
void BGMManager::Teardown() {
	currentSource->Teardown();
}
/*
 *	BGM‚ÌÄ¶ˆ—
 *	@param[in]	int setVolume	‰¹—Ê
 */
void BGMManager::PlayBGM(const int setVolume) {
	currentSource->Play(setVolume);
}
/*
 *	BGM‚Ì’â~ˆ—
 */
void BGMManager::StopBGM() {
	currentSource->Stop();
}
/*
 *	BGM‚Ì•ÏXˆ—
 *	@param[in]	const std::string setBGMName	İ’è‚·‚éBGM–¼
 */
void BGMManager::ChangeBGM(const std::string setBGMName) {
	// ‰¹Œ¹‚Ì’â~A”jŠüˆ—
	currentSource->Stop();
	currentSource->Teardown();
	// ‰¹Œ¹‚Ìİ’è
	int setHandle = bgmHandleMap[setBGMName];
	currentSource->SetAudioHandle(setHandle);
}
/*
 *	BGMƒnƒ“ƒhƒ‹‚Ì“o˜^
 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éBGM–¼
 *	@param[in]	const int& setHandle			“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
 */
void BGMManager::RegisterBGMHandle(const std::string& setKeyName, const int& setHandle) {
	if (!ExistBGMHandle(setKeyName)) return;

	bgmHandleMap[setKeyName] = setHandle;
}
/*
 *	BGMƒnƒ“ƒhƒ‹‚ª“o˜^Ï‚İ‚©”»’è
 *  @param[in]	const std::string& setKeyName	 ’²‚×‚éBGM–¼
 *  @return		bool
 */
bool BGMManager::ExistBGMHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	return bgmHandleMap.find(setKeyName) != bgmHandleMap.end();
}
/*
 *	BGMƒnƒ“ƒhƒ‹‚Ìæ“¾
 *	@param[in]	const std::string& setKeyName	’²‚×‚éBGM–¼
 *	@return		int
 */
int BGMManager::GetBGMHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	auto it = bgmHandleMap.find(setKeyName);
	if (it != bgmHandleMap.end()) return it->second;
	return -1;
}
