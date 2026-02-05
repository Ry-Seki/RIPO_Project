/*
 *	@file	SEManager.cpp
 *	@author	Seki
 */

#include "SEManager.h"

/*
 *	@brief	デストラクタ
 */
SEManager::~SEManager() {
	for (auto itr : seHandleMap) {
		int handle = itr.second;
		if(handle != -1) DeleteSoundMem(handle);
	}
	seHandleMap.clear();
}
/*
 *	@brief	初期化処理
 */
void SEManager::Initialize() {
	// 未使用リストの初期化
	for (int i = 0; i < GameConst::MAX_SE_INDEX; i++) {
		unuseSEList.push_back(std::make_shared<SESource>());
	}
}
/*
 *	@brief	ロード済みのデータの設定 (コールバック)
 */
void SEManager::SetupData() {
}
/*
 *	@brief	更新処理
 */
void SEManager::Update() {
	// 使用リストの再生中フラグを取得
	for (auto itr = useSEList.begin(); itr != useSEList.end();) {
		auto SE = *itr;

		// 再生が終わっていたら破棄処理を行う
		if (!SE->IsPlaying()) {
			SE->Teardown();
			unuseSEList.push_back(SE);
			itr = useSEList.erase(itr);
		}else {
			itr++;
		}
	}
}
/*
 *	@brief		SE再生処理
 *  @param[in]	const std::string& setKeyName	再生するSE名
 *  @param[in]	float setVolume					音量
 */
void SEManager::PlaySE(const std::string& setKeyName, float setVolume) {
	// 未使用リストの中身が空なら新たに生成
	if (unuseSEList.empty()) {
		unuseSEList.push_back(std::make_shared<SESource>());
	}
	// 未使用リストから取得
	std::shared_ptr<SESource> useSE = unuseSEList.front();

	// 音源ハンドルの取得
	int seHandle = GetSEHandle(setKeyName);
	// 音源ハンドルの設定
	useSE->SetAudioHandle(seHandle);
	// 未使用リストの先頭要素削除
	unuseSEList.pop_front();
	// 使用リストに追加
	useSEList.push_back(useSE);

	// SEの再生
	useSE->Play(setVolume);
}
/*
 *	@brief		SE破棄処理
 *  @param[in]	std::shared_ptr<SESource> destroySE
 */
void SEManager::TeardownSE(std::shared_ptr<SESource> destroySE) {
	if (!destroySE) return;
	// 破棄処理
	destroySE->Teardown();
}
/*
 *	@brief		SEハンドルの登録
 *	@param[in]	const std::string&	setKeyName	登録するSE名
 *	@param[in]	const int setHandle				登録する音源ハンドル
 */
void SEManager::RegisterSEHandle(const std::string& setKeyName, const int setHandle) {
	if (ExistSEHandle(setKeyName)) return;

	seHandleMap[setKeyName] = setHandle;
}
/*
 *	@brief		SEハンドルが登録済みか判定
 *  @param[in]	const std::string& setKeyName	調べるSE名
 *  @return		bool
 */
bool SEManager::ExistSEHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	return seHandleMap.find(setKeyName) != seHandleMap.end();
}
/*
 *	@brief		SEハンドルの取得
 *	@param[in]	const std::string& setKeyName	調べるSE名
 *	@return		int
 */
int SEManager::GetSEHandle(const std::string& setKeyName) const {
	std::lock_guard<std::mutex> lock(mtx);
	auto it = seHandleMap.find(setKeyName);
	if (it != seHandleMap.end()) return it->second;
	return -1;
}
