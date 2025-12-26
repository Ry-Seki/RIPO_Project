/*
 *	@file	SelectionShop.h
 *	@author	Seki
 */

#ifndef _SELECTION_SHOP_H_
#define _SELECTION_SHOP_H_

#include "../SelectionBase.h"
#include "../../../../Data/ItemCatalogData.h"

#include <vector>

/*
 *	@brief	ショップ選択クラス
 */
class SelectionShop : public SelectionBase {
private:
	std::vector<ItemData*> itemDataList;	// アイテムデータリスト
	std::vector<int> buyItemIDList;			// 購入したアイテムIDリスト
	bool inputHandle = false;				// 入力フラグ
	bool isLoad = false;					// ロードフラグ

	std::function<void(Engine& engine, std::vector<int>)> ActiveShop;	// 行動管理クラスのショップ開放関数

public:
	/*
	 *	@brief	コンストラクタ
	 */
	SelectionShop() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SelectionShop() override {}

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup(Engine& engine) override;
	/*
	 *	@brief	ロード済みデータのセット（コールバック）
	 */
	void SetupData(Engine& engine);
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
	/*
	 *  @brief  行動実行関数の呼び出し
	 */
	void SetAction() override;

public:
	/*
	 *	@brief		ActionManagerのショップ管理関数の設定
	 *	@param[in]	std::function<void(Engine& engine, std::vector<int>)> setActiveDungeon	行動管理クラスのショップ解放関数
	 */
	inline void SetActiveShop(std::function<void(Engine& engine, std::vector<int>)> setActiveShop) {
		ActiveShop = setActiveShop;
	}
};

#endif // !_SELECTION_SHOP_H_