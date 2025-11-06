/*
 *	@file	StageUtility.h
 *	@author	Seki
 */

#ifndef _STAGE_UTILITY_H_
#define _STAGE_UTILITY_H_

#include "../Manager/StageManager.h"

/*
 *	ステージ関連実行処理
 */
class StageUtility {
public:
	/*
	 *  現在のステージの取得
	 *  @return		StageBase*
	 */
	static StageBase* GetCurrentStage() { return StageManager::GetInstance().GetCurrentStage(); }
	/*
	 *	ひとつ前のステージの取得
	 *  @return		StageBase*
	 */
	static StageBase* GetPrevStage() { return StageManager::GetInstance().GetPrevStage(); }
	/*
	 *	ステージのFrameを取得
	 *  @param[in]	const std::string& frameName
	 *  @return		int
	 */
	static int GetStageFrame(const std::string& frameName) { return StageManager::GetInstance().GetStageFrame(frameName); }

public:
	/*
	 *	スタート位置の取得
	 *  @return		Vector3
	 */
	static Vector3 GetStartPos() { return StageManager::GetInstance().GetStartPos(); }
	/*
	 *	ゴール位置の取得
	 *  @return		Vector3
	 */
	static Vector3 GetGoalPos() { return StageManager::GetInstance().GetGoalPos(); }
	/*
	 *	敵の初期生成位置の取得
	 *	@return		std::vector<Vector3>
	 */
	static std::vector<Vector3> GetEnemySpwanPos() { return StageManager::GetInstance().GetEnemySpwanPos(); }
	/*
	 *  お宝の生成位置の取得
	 *  @return		std::vector<Vector3>
	 */
	static std::vector<Vector3> GetTreasureSpwanPos() { return StageManager::GetInstance().GetTreasureSpwanPos(); }
	/*
	 *	ポイントライト生成位置の取得
	 *  @return		std::vector<Vector3>
	 */
	static std::vector<Vector3> GetPointLightPos() { return StageManager::GetInstance().GetPointLightPos(); }
	/*
	 *	階層移動用階段位置の取得
	 *  @return		Vector3
	 */
	static Vector3 GetStairsPos() { return StageManager::GetInstance().GetStairsPos(); }

public:
	/*
	 *	jsonの変更
	 *  @param[in]	JSON setJSON
	 */
	static void SetStageJSONData(JSON setJSON) { StageManager::GetInstance().SetStageJSONData(setJSON); }
};

#endif // !_STAGE_UTILITY_H_
