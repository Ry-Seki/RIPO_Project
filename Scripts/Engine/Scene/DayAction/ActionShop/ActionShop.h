/*
 *	@file	ActionShop.h
 *	@author	Seki
 */

#ifndef _ACTION_SHOP_H_
#define _ACTION_SHOP_H_

#include "../DayActionBase.h"

#include <vector>
#include <memory>

/*
 *	ショップアクションクラス
 */
class ActionShop : public DayActionBase {
private:
    std::vector<int> itemIDList;    // 購入したアイテムIDリスト
    float timer = 0.0f;
    float limitTime = 7.0f;

public:
    /*
     *  コンストラクタ
     */
    ActionShop() : DayActionBase("Shop") {}
    /*
     *  デストラクタ
     */
    ~ActionShop() override {}

public:
    /*
     *  初期化処理
     */
    void Initialize(Engine& engine) override;
    /*
     *  ロード済みのデータをセット(コールバック)
     */
    void SetupData(Engine& engine) override;
    /*
     *  更新処理
     */
    void Update(Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;
    /*
     *  破棄処理
     */
    void Teardown() override;

public:
    /*
     *  @brief      購入したアイテムIDリストの設定
     *  @param[in]  const std::vector<int>& setItemIDList
     */
    inline void SetItemIDList(const std::vector<int>& setItemIDList){
        itemIDList = setItemIDList;
    }

};
#endif // !_ACTION_SHOP_H_
