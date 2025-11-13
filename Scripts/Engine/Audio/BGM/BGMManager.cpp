/*
 *	@file	BGMManager.cpp
 *	@author	Seki
 */

#include "BGMManager.h"

/*
 *	デストラクタ
 */
BGMManager::~BGMManager() {
	for (auto itr : bgmHandleMap) {
		DeleteSoundMem(itr.second);
	}
	bgmHandleMap.clear();
}

/*
 *	初期化処理
 */
void BGMManager::Initialize() {
}
/*
 *	ロード済みのデータの設定(コールバック)
 */
void BGMManager::SetupData() {
}
/*
 *	更新処理
 */
void BGMManager::Update() {
}
/*
 *	描画処理
 */
void BGMManager::Render() {
}
/*
 *	BGMの破棄処理
 */
void BGMManager::Teardown() {
	currentSource->Teardown();
}
/*
 *	BGMの再生処理
 *	@param[in]	int setVolume	音量 (0～100)
 */
void BGMManager::PlayBGM(const int setVolume) {
	currentSource->Play(setVolume);
}
/*
 *	BGMの停止処理
 */
void BGMManager::StopBGM() {
	currentSource->Stop();
}
/*
 *	BGMの変更処理
 *	@param[in]	const std::string setBGMName	設定するBGM名
 */
void BGMManager::ChangeBGM(const std::string setBGMName) {
	if (!currentSource) {
		// 音源の停止、破棄処理
		currentSource->Stop();
		currentSource->Teardown();
	}
	// 音源の設定
	int setHandle = GetBGMHandle(setBGMName);
	currentSource->SetAudioHandle(setHandle);
}
/*
 *	BGMハンドルの登録
 *	@param[in]	const std::string&	setKeyName	登録するBGM名
 *	@param[in]	const int setHandle				登録する音源ハンドル
 */
void BGMManager::RegisterBGMHandle(const std::string& setKeyName, const int setHandle) {
	if (ExistBGMHandle(setKeyName)) return;

	bgmHandleMap[setKeyName] = setHandle;
}
/*
 *	BGMハンドルが登録済みか判定
 *  @param[in]	const std::string& setKeyName	 調べるBGM名
 *  @return		bool
 */
bool BGMManager::ExistBGMHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	return bgmHandleMap.find(setKeyName) != bgmHandleMap.end();
}
/*
 *	BGMハンドルの取得
 *	@param[in]	const std::string& setKeyName	調べるBGM名
 *	@return		int
 */
int BGMManager::GetBGMHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	auto it = bgmHandleMap.find(setKeyName);
	if (it != bgmHandleMap.end()) return it->second;
	return -1;
}
