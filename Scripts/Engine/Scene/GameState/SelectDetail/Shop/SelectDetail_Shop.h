/*
 *	@file	SelectDetail_Shop.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_SHOP_H_
#define _SELECT_DETAIL_SHOP_H_

#include "../SelectDetailBase.h"
#include "../../../../../Data/ItemCatalogData.h"

#include <vector>

/*
 *	@brief	買い物の内容を選択するクラス
 */
class SelectDetail_Shop : public SelectDetailBase {
private:
	std::vector<ItemData*> itemDataList;
	std::vector<int> buyItemIDList;
	const int startX = 180;
	const int intervalX = 40; 

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

};

#endif // !_SELECT_DETAIL_SHOP_H_