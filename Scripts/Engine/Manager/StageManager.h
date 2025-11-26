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
#include "../Component/Character/ArmActionComponent.h"
#include "../Component/Character/CharacterUtility.h"
#include <memory>
#include <string>

using namespace CharacterUtility;
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

	/*
	 *	簡易的に現在のステージにアクセス
	 *  @tips	Update,Render,StageCollider,を引数に持たせる
	 */
	template<typename Func>
	void WithCurrentStage(Func&& func) {
		// ステージがあるか見る
		if (!loadedStage)return;

		// 渡された関数を実行
		func(*loadedStage);
	}

public:

	/*
	 *  初期化
	 */
	void Initialize(Engine& setEngine);

	/*
	 *	ステージの読み込み
	 *  @param	modelHandleBase		モデルハンドルの配列
	 */
	void LoadStage(const std::vector<int> modelHandleBase);

	void ChangeStage();


	/*
	 * @brief 次のステージへ進む
	 */
	void NextStage(int setID);

	/*
	 * @brief 前のステージに戻る
	 */
	void PrevStage();

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
	void StageCollider(GameObject* other, Vector3 MoveVec);


public:

	/*
	 *	現在ステージハンドルの取得
	 */
	int GetCurrentStageHandle() const { return stageState.GetCurrentStageHandle(); }

	/*
	 *	ひとつ前のステージハンドルの取得
	 */
	int GetPrevStageHandle() const { return stageState.GetPrevStageHandle(); }

	/*
	 *  現在のステージの実態の取得
	 */
	StageBase* GetCurrentStage() const { return loadedStage.get(); }

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

	/*
	 *	プレイヤーが保持しているお宝を取得する関数を呼ぶ
	 */
	GameObject* GetLiftObject() {
		// プレイヤーを取得
		auto player = GetPlayer();
		if (!player)return;

		// お宝を取得
		auto treasure = player->GetComponent<ArmActionComponent>()->GetLiftObject();
		if (!treasure)return;

		// 取得したお宝を返す
		return treasure;

	}

};


#endif // !_STAGEMANAGER_H_

