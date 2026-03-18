/*
 *	@file	StageUtility.h
 *	@author	Seki
 */

#ifndef _STAGE_UTILITY_H_
#define _STAGE_UTILITY_H_

#include "../Manager/StageManager.h"

 /*
  *	ステージ関連実行処理
  *  @note	名前空間での省略可能	using namespace StageUtility
  */
namespace StageUtility {
	/*
	 *	ステージの読み込み
	 *  @param[in]	int modelHandleBase		モデルハンドル
	 */
	inline void LoadStage(const std::vector<int> stageHandle) {
		StageManager::GetInstance().LoadStage(stageHandle);
	}

	/*
	 *  現在のステージの取得
	 *  @return		StageBase*
	 */
	inline StageBase* GetCurrentStage() {
		return StageManager::GetInstance().GetCurrentStage();
	}

	/*
	 *	jsonの変更
	 *  @param[in]	JSON setJSON
	 */
	inline void SetStageJSONData(JSON setJSON) {
		StageManager::GetInstance().SetStageJSONData(setJSON);
	}

	inline int GetCurrentStageHandle() {
		return StageManager::GetInstance().GetCurrentStageHandle();
	}

	/*
	 *	ライトの座標設定
	 *  @param[in]	setValue	ライトの座標
	 */
	inline void SetLightPos(std::vector<Vector3> setValue) {
		StageManager::GetInstance().SetLightPos(setValue);
	}
};

#endif // !_STAGE_UTILITY_H_
