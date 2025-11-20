/*
 *	@file	ItemManager.h
 *  @author	Seki
 */

#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

#include "../../Singleton.h"
#include "../../../Data/ItemCatalogData.h"
#include "../../JSON.h"

#include <memory>

/*
 *	@brief	プレイヤーが所持しているアイテム管理クラス
 */
class ItemManager : public Singleton<ItemManager>{
	friend class Singleton<ItemManager>;

private:
	std::vector<std::shared_ptr<ItemData>> playerWeaponList;	// 武器の種類
	std::vector<std::shared_ptr<ItemData>> mapList;				// 地図の種類

public:
	/*
	 *	@brief	コンストラクタ
	 */
	ItemManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~ItemManager() = default;

public:
	/*
	 *	@brief		初期化処理
	 */
	void Initialize();
	/*
	 *	@brief		ロード済みデータのセット(コールバック)
	 *	@param[in]	const JSON& setJSON
	 */
	void SetupData(const JSON& setJSON);

};

#endif // !_ITEM_MANAGER_H_
