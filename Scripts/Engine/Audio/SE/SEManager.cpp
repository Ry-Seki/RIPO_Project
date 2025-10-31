/*
 *	@file	SEManager.cpp
 *	@author	Seki
 */

#include "SEManager.h"

/*
 *	初期化処理
 */
void SEManager::Initialize() {
	// 未使用リストの初期化
	for (int i = 0; i < GameConst::_MAX_SE_INDEX; i++) {
		unuseSEList.push_back(std::make_shared<SESource>());
	}
}
/*
 *	ロード済みのデータの設定(コールバック)
 */
void SEManager::Setup() {
}
/*
 *	更新処理
 */
void SEManager::Update() {
	// 使用リストの再生中フラグを取得
	for (auto itr = useSEList.begin(); itr != useSEList.end();) {
		auto se = *itr;

		// 再生が終わっていたら、破棄処理を行う
		if (!se->IsPlaying()) {
			se->Teardown();
			unuseSEList.push_back(se);
			itr = useSEList.erase(itr);
		}else {
			itr++;
		}
	}
}
/*
 *	SE再生処理
 *  @param[in]	const int setHandle		再生する音源ハンドル
 *  @param[in]	const int setVolume		音量
 */
void SEManager::PlaySE(const int setHandle, const int setVolume) {
	// 未使用リストの中身が空なら新たに生成
	if (unuseSEList.empty()) {
		unuseSEList.push_back(std::make_shared<SESource>());
	}
	// 未使用リストから取得
	std::shared_ptr<SESource> useSE = unuseSEList.front();
	// 音源ハンドルの設定
	useSE->SetAudioHandle(setHandle);
	// 未使用リストの先頭要素削除
	unuseSEList.pop_front();
	// 使用リストに追加
	useSEList.push_back(useSE);
	// SEの再生
	useSE->Play(setVolume);
}
/*
 *	SE破棄処理
 *  @param[in]	std::shared_ptr<SESource> destroySE
 */
void SEManager::TeardownSE(std::shared_ptr<SESource> destroySE) {
	if (!destroySE) return;
	// 破棄処理
	destroySE->Teardown();
}
