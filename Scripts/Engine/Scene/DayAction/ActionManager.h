/*
 *	@file	ActionManager.h
 *	@author	Seki
 */

#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "../../Singleton.h"
#include "DayActionBase.h"
#include "ActionDungeon/ActionDungeon.h"
#include "../../../Data/DungeonStageData.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

/*
 *  各アクションの管理クラス 
 */
class ActionManager : public Singleton<ActionManager>{
    // フレンド宣言
    friend class Singleton<ActionManager>;

private:
    DayActionPtr actionBase;                // アクションクラスのオリジナル
    DayActionPtr currentAction;             // 現在のアクション
    std::function<void()> onComplete;       // アクション完了コールバック
    bool isActive = false;                  // 処理有効フラグ

private:
    /*
     *  コンストラクタ
     */
    ActionManager() : actionBase(std::make_shared<ActionDungeon>()) {}
    /*
     *  デストラクタ
     */
    ~ActionManager() = default;

public:
    /*
     *  更新処理
     */
    inline void Update(Engine& engine, float deltaTime) {
        if (!isActive) return;

        if (currentAction) currentAction->Update(engine,deltaTime);
    }
    /*
     *  描画処理
     */
    inline void Render() {
        if (!isActive) return;

        if (currentAction) currentAction->Render();
    }

public:
    /*
     *  ダンジョンアクション開始
     */
    void ActiveDungeon(Engine& engine, DungeonStageData setStageData);
    void DebugActiveDungeon(Engine& engine, std::string setFilePath);
    /*
     *  トレーニングアクション開始
     */
    void ActiveTraining(Engine& engine);
    /*
     *  ショップアクション開始
     */
    void ActiveShop(Engine& engine);
    /*
     *  アルバイトアクション開始
     */
    void ActivePartTime(Engine& engine);

public:
    /*
     *  コールバックの設定
     *  param[in]   std::function<void()> callback      コールバック
     */
    inline void SetOnComplete(std::function<void()> callback) { onComplete = callback; }
    /*
     *  処理有効フラグの取得
     */
    inline bool IsActive() const { return isActive; }
};

#endif // !_ACTION_MANAGER_H_