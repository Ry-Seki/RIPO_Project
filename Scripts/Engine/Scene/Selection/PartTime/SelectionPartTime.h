/*
 *	@file	SelectionPartTime.h
 *	@author	Seki
 */

#ifndef _SELECTION_PART_TIME_H_
#define _SELECTION_PART_TIME_H_

#include "../SelectionBase.h"

class SelectionPartTime : public SelectionBase {
private:

	bool inputHandle = false;				// 入力フラグ
	bool isLoad = false;					// ロードフラグ

	std::function<void(Engine& engine, int)> ActivePartTime;	// 行動管理クラスのアルバイト開放関数

public:
	/*
	 *	@brief	コンストラクタ
	 */
	SelectionPartTime() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SelectionPartTime() override = default;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	ロード済みデータのセット（コールバック）
	 */
	void SetupData(Engine& engine) override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

public:
	/*
	 *	@brief		ActionManagerのアルバイト管理関数の設定
	 *	@param[in]	std::function<void(Engine& engine, int)> setActivePartTime	行動管理クラスのアルバイト解放関数
	 */
	inline void SetActivePartTime(std::function<void(Engine& engine, int)> setActivePartTime) {
		ActivePartTime = setActivePartTime;
	}
};

#endif // !_SELECTION_PART_TIME_H_