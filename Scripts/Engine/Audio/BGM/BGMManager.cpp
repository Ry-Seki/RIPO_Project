/*
 *	@file	BGMManager.cpp
 *	@author	Seki
 */

#include "BGMManager.h"

/*
 *	@brief	デストラクタ
 */
BGMManager::~BGMManager() {
	for (auto itr : bgmHandleMap) {
		int handle = itr.second;
		if(handle != -1) DeleteSoundMem(handle);
	}
	bgmHandleMap.clear();
}
/*
 *	@brief	初期化処理
 */
void BGMManager::Initialize() {
}
/*
 *	@brief	ロード済みのデータの設定(コールバック)
 */
void BGMManager::SetupData() {
}
/*
 *	@brief	描画処理
 */
void BGMManager::Render() {
}
/*
 *	@brief	BGMの破棄処理
 */
void BGMManager::Teardown() {
	currentSource->Teardown();
}
/*
 *	@brief		BGMの再生処理
 *  @param[in]	float setVolume		音量
 */
void BGMManager::PlayBGM(float setVolume) {
	currentSource->Play(setVolume);
}
/*
 *	@brief		BGMの停止処理
 */
void BGMManager::StopBGM() {
	currentSource->Stop();
}
/*
 *	@brief		BGMの変更処理
 *	@param[in]	const std::string setBGMName	設定するBGM名
 */
void BGMManager::ChangeBGM(const std::string setBGMName) {
	if (currentSource) {
		// 音源の停止、破棄処理
		currentSource->Stop();
		currentSource->Teardown();
	}
	// 音源の設定
	int setHandle = GetBGMHandle(setBGMName);
	currentSource->SetAudioHandle(setHandle);
}
/*
 *	@brief		音量の更新処理
 *	@param[in]	float setVolume
 */
void BGMManager::UpdateVolume(float setVolume) {
	if(currentSource) currentSource->ChangeAudioVolume(setVolume);
}
/*
 *	@brief		BGMハンドルの登録
 *	@param[in]	const std::string&	setKeyName	登録するBGM名
 *	@param[in]	const int setHandle				登録する音源ハンドル
 */
void BGMManager::RegisterBGMHandle(const std::string& setKeyName, const int setHandle) {
	if (ExistBGMHandle(setKeyName)) return;

	bgmHandleMap[setKeyName] = setHandle;
}
/*
 *	@brief		BGMハンドルが登録済みか判定
 *  @param[in]	const std::string& setKeyName	 調べるBGM名
 *  @return		bool
 */
bool BGMManager::ExistBGMHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	return bgmHandleMap.find(setKeyName) != bgmHandleMap.end();
}
/*
 *	@brief		BGMハンドルの取得
 *	@param[in]	const std::string& setKeyName	調べるBGM名
 *	@return		int
 */
int BGMManager::GetBGMHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	auto it = bgmHandleMap.find(setKeyName);
	if (it != bgmHandleMap.end()) return it->second;
	return -1;
}
