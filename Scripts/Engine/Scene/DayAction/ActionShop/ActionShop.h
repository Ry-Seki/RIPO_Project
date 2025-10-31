/*
 *	@file	ActionShop.h
 *	@author	Seki
 */

#ifndef _ACTION_SHOP_H_
#define _ACTION_SHOP_H_

#include "../DayActionBase.h"
#include "../../../Load/CSV/LoadCSV.h"
#include "../../../Load/Sprite/LoadSprite.h"
#include "../../../Load/LoadManager.h"

#include <memory>

/*
 *	ショップアクションクラス
 */
class ActionShop : public DayActionBase {
private:
    std::shared_ptr<LoadCSV> itemData;          // アイテムデータ
    std::shared_ptr<LoadSprite> itemImages;     // アイテム画像
    std::shared_ptr<LoadCSV> itemDescriptions;  // アイテム説明
    std::shared_ptr<LoadCSV> shopDialogue;      // セリフ

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
    void Setup(Engine& engine) override;
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
};
#endif // !_ACTION_SHOP_H_
