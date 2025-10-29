/*
 *	@file StageManager.h
 *  @author oorui
 */

#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_


#include "../Singleton.h"
#include "../Engine.h"
#include "../Stage/Stage.h"
#include <memory>
#include <string>
#include "../Load/LoadManager.h"
#include "../Load/JSON/LoadJSON.h"
#include "../VecMath.h"

 /*
  *	ステージ全体の管理
  */
class StageManager : public Singleton<StageManager> {
	// フレンド宣言
	friend class Singleton<StageManager>;

private:

	Engine* engine;						// ゲームエンジン参照
	std::unique_ptr<StageBase> pStage;	// 現在のステージ(自動的に削除される*)
	JSON json;

	StageManager();						// コンストラクタ
	~StageManager() = default;			// デストラクタ

public:

	/*
	 *  初期化
	 */
	void Initialize(Engine& setEngine);

	void LoadStage(const int modelHandleBase);

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

	void StageCollider(Vector3* position, Vector3 MoveVec);

public:
	/*
	 *  現在のステージの取得
	 */
	StageBase* GetStage() const { return pStage.get(); }

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
	 *	jsonの変更
	 */
	void SetStageJSONData(JSON setJSON) { json = setJSON; }

};


#endif // !_STAGEMANAGER_H_

