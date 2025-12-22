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
	 *	スタート位置の取得
	 *  @return		Vector3
	 */
	inline Vector3 GetStartPos() {
		return StageManager::GetInstance().GetStartPos();
	}
	/*
	 *	ゴール位置の取得
	 *  @return		Vector3
	 */
	inline std::vector<Vector3> GetGoalPos() {
		return StageManager::GetInstance().GetGoalPos();
	}
	/*
	 *	敵の初期生成位置の取得
	 *  @param[out]	std::vector<int>& IDList
	 *	@return		std::vector<Vector3>
	 */
	inline std::vector<Vector3> GetEnemySpwanPos(std::vector<int>& IDList) {
		return StageManager::GetInstance().GetEnemySpwanPos(IDList);
	}
	/*
	 *  お宝の生成位置の取得
	 *  @return		std::vector<Vector3>
	 */
	inline std::vector<Vector3> GetTreasureSpwanPos() {
		return StageManager::GetInstance().GetTreasureSpwanPos();
	}
	/*
	 *	ポイントライト生成位置の取得
	 *  @return		std::vector<Vector3>
	 */
	inline std::vector<Vector3> GetPointLightPos() {
		return StageManager::GetInstance().GetPointLightPos();
	}
	/*
	 *	階層移動用階段位置の取得
	 *  @return		Vector3
	 */
	inline std::vector<Vector3> GetStairsPos() {
		return StageManager::GetInstance().GetStairsPos();
	}

	/*
	 *	リスポーン位置の取得
	 */
	inline std::vector<Vector3> GetRespawnPos() {
		return StageManager::GetInstance().GetRespawnPos();
	}

	/*
	 *	jsonの変更
	 *  @param[in]	JSON setJSON
	 */
	inline void SetStageJSONData(JSON setJSON) {
		StageManager::GetInstance().SetStageJSONData(setJSON);
	}
};

#endif // !_STAGE_UTILITY_H_
