/*
 *	@file	ActionManager.h
 *	@author	Seki
 */

#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "../../Singleton.h"
#include "DayActionBase.h"
#include "../../../Data/DungeonStageData.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

enum class ActionType {
    DungeonAction,
    TrainingAction,
    ShopAction,
    PartTimeAction,
};

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

private:
    /*
     *  コンストラクタ
     */
    ActionManager() = default;
    /*
     *  デストラクタ
     */
    ~ActionManager() = default;

public:
    /*
     *  更新処理
     */
    void Update(Engine& engine, float deltaTime) {
        if (currentAction) currentAction->Update(engine,deltaTime);
    }
    /*
     *  描画処理
     */
    void Render() {
        if (currentAction) currentAction->Render();
    }

public:
    /*
     *  ダンジョンアクション開始
     */
    void ActiveDungeon(Engine& engine, DungeonStageData setStageData);
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
     */
    inline void SetOnComplete(std::function<void()> callback) { onComplete = callback; }
};

#endif // !_ACTION_MANAGER_H_