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
 *	@brief  ショップアクションクラス
 */
class ActionShop : public DayActionBase {
private:
    std::vector<int> itemIDList;    // 購入したアイテムIDリスト
    float timer = 0.0f;
    float limitTime = 3.0f;

public:
    /*
     *  @brief  コンストラクタ
     */
    ActionShop() : DayActionBase("Shop") {}
    /*
     *  @brief  デストラクタ
     */
    ~ActionShop() override {}

public:
    /*
     *  @brief  初期化処理
     */
    void Initialize(Engine& engine) override;
    /*
     *  @brief  準備前処理
     */
    void Setup(Engine& engine) override;
    /*
     *  @brief  ロード済みのデータをセット(コールバック)
     */
    void SetupData(Engine& engine);
    /*
     *  @brief  更新処理
     */
    void Update(Engine& engine, float deltaTime) override;
    /*
     *  @brief  描画処理
     */
    void Render() override;
    /*
     *  @brief  破棄処理
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
