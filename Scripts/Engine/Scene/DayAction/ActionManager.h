/*
 *	@file	ActionManager.h
 *	@author	Seki
 */

#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "DayActionBase.h"
#include "ActionFactory.h"
#include "../../../Data/DungeonStageData.h"
#include "../../../Data/DungeonFloorData.h"
#include "../../Load/JSON/LoadJSON.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

/*
 *  各アクションの管理クラス 
 */
class ActionManager{
private:
    DayActionPtr currentAction;             // 現在のアクション
    std::function<void()> onComplete;       // アクション完了コールバック
    bool isActive = false;                  // 処理有効フラグ

public:
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
     *  初期化処理
     *  @param[in]  JSON setJSON    セットするJSONデータ
     */
    void Initialize(JSON setJSON);
    /*
     *  更新処理
     */
    void Update(Engine& engine, float deltaTime);
    /*
     *  描画処理
     */
    void Render();

public:
    /*
     *  @brief      ダンジョンアクション開始
     */
    void ActiveDungeon(Engine& engine, DungeonStageData setStageData, DungeonFloorData setFloorData);
    void DebugActiveDungeon(Engine& engine, DungeonStageData setStageData, DungeonFloorData setFloorData);
    /*
     *  @brief      トレーニングアクション開始
     *  @param[in]  const int setActionNum    アクション番号
     */
    void ActiveTraining(Engine& engine, const int setActionNum = -1);
    /*
     *  @brief      ショップアクション開始
     *  @param[in]  const std::vector<int>& itemIDList     アイテムIDリスト
     */
    void ActiveShop(Engine& engine, const std::vector<int>& itemIDList);
    /*
     *	@brief      アルバイトアクション開始
     *  @param[in]	const int incomeValue		アルバイトの収入
     */
    void ActivePartTime(Engine& engine, const int incomeValue);

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