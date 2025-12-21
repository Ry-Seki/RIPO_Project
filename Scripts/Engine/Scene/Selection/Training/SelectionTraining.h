/*
 *	@file	SelectionTraining.h
 *	@author	Seki
 */

#ifndef _SELECTION_TRAINING_H_
#define _SELECTION_TRAINING_H_

#include "../SelectionBase.h"

/*
 *	@brief	トレーニング選択クラス
 */
class SelectionTraining : public SelectionBase {
private:
	bool inputHandle = false;							// 入力フラグ
	bool isLoad = false;								// ロードフラグ
	
	std::function<void(Engine& engine, int)> ActiveTraining;	// 行動管理クラスのトレーニング開放関数

public:
	/*
	 *	@brief	デストラクタ
	 */
	~SelectionTraining() override {}

public:
	/*
	 *	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup(Engine& engine) override;
	/*
	 *	ロード済みデータのセット（コールバック）
	 */
	void SetupData(Engine& engine);
	/*
	 *	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;


public:
	/*
	 *	ActionManagerのトレーニング管理関数の設定
	 *	@param[in]	std::function<void(Engine& engine, int)> setActiveDungeon	行動管理クラスのトレーニング解放関数
	 */
	inline void SetActiveTraining(std::function<void(Engine& engine, int)> setActiveTraining) {
		ActiveTraining = setActiveTraining;
	}
};
#endif // !_SELECTION_TRAINING_H_