/*
 *	@file StageManager.h
 *  @author oorui
 */

#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_


#include "../Singleton.h"
#include "../Engine.h"
#include "../Stage/Stage.h"
#include "../Load/LoadManager.h"
#include "../Load/JSON/LoadJSON.h"
#include "../VecMath.h"
#include "../Stage/StageState.h"
#include <memory>
#include <string>

 /*
  *	ステージ全体の管理
  */
class StageManager : public Singleton<StageManager> {
	// フレンド宣言
	friend class Singleton<StageManager>;

private:

	Engine* engine;						// ゲームエンジン参照
	JSON json;							// JSONデータ
	StageState stageState;				// ステージの状態保持

	std::unique_ptr<StageBase> loadedStage;	// 読み込み済みステージデータ

private:
	StageManager();						// コンストラクタ
	~StageManager() = default;			// デストラクタ

private:
	template<typename Func>
	void WithCurrentStage(Func&& func) {
		if (auto* stage = stageState.GetCurrentStage()) {
			func(*stage);
		}
	};

public:

	/*
	 *  初期化
	 */
	void Initialize(Engine& setEngine);

	/*
	 *	ステージの読み込み
	 *  @param	modelHandleBase		モデルハンドル
	 */
	void LoadStage(const int modelHandleBase);

	void ChangeStage();

	/*
	 *  更新
	 */
	void Update();

	/*
	 *  描画
	 */
	void Render();

	/*
	 *  終了
	 */
	void Execute();

	/*
	 *	ステージの当たり判定
	 *  @param position		対象者の座標
	 *  @param MoveVec		対象者の移動量
	 */
	void StageCollider(Vector3* position, Vector3 MoveVec);

	/*
	 *	前のステージに戻す
	 */
	void RestorePrevStage();

public:
	/*
	 *  現在のステージの取得
	 */
	StageBase* GetCurrentStage() const { return stageState.GetCurrentStage(); }

	/*
	 *	ひとつ前のステージの取得
	 */
	StageBase* GetPrevStage() const { return stageState.GetPrevStage(); }

	/*
	 *	ステージのFrameを取得
	 */
	int GetStageFrame(const std::string& frameName) const;

	/*
	 *	スタート位置の取得
	 */
	Vector3 GetStartPos()const;

	/*
	 * ゴール位置の取得
	 */
	Vector3 GetGoalPos()const;

	/*
	 *	敵の初期生成位置の取得
	 */
	std::vector<Vector3> GetEnemySpwanPos()const;

	/*
	 * お宝の生成位置の取得
	 */
	std::vector<Vector3> GetTreasureSpwanPos()const;

	/*
	 *	ポイントライト生成位置の取得
	 */
	std::vector<Vector3> GetPointLightPos()const;

	/*
	 *	階層移動用階段位置の取得
	 */
	Vector3 GetStairsPos()const;

	/*
	 *	jsonの変更
	 */
	void SetStageJSONData(JSON setJSON) { json = setJSON; }

};


#endif // !_STAGEMANAGER_H_

