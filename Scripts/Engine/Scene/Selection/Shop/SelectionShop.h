/*
 *	@file	SelectionShop.h
 *	@author	Seki
 */

#ifndef _SELECTION_SHOP_H_
#define _SELECTION_SHOP_H_

#include "../SelectionBase.h"

class SelectionShop : public SelectionBase {
private:
	bool inputHandle = false;								// 入力フラグ
	bool isLoad = false;									// ロードフラグ

	std::function<void(Engine& engine, int)> ActiveShop;	// 行動管理クラスのダンジョン開放関数

public:
	/*
	 *	@brief	コンストラクタ
	 */
	SelectionShop() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SelectionShop() override = default;

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

public:
	/*
	 *	@brief		ActionManagerのショップ管理関数の設定
	 *	@param[in]	std::function<void()> setActiveDungeon	行動管理クラスのショップ解放関数
	 */
	inline void SetActiveTraining(std::function<void(Engine& engine, int)> setActiveShop) {
		ActiveShop = setActiveShop;
	}
};

#endif // !_SELECTION_SHOP_H_