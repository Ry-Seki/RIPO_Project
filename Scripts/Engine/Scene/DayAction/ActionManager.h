/*
 *	@file	ActionManager.h
 *	@author	Seki
 */

#ifndef _ACTION_MANAGER_H_
#define _ACTION_MANAGER_H_

#include "../../JSON.h"
#include "../../GameEnum.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

// 前方宣言
class Engine;
class DayActionBase;
class DungeonStageData;
class DungeonFloorData;

/*
 *  各アクションの管理クラス 
 */
class ActionManager{
private:
    std::unordered_map<GameEnum::ActionType,
        std::shared_ptr<DayActionBase>> actionMap;	    // 選択クラスリスト

    std::shared_ptr<DayActionBase> currentAction;       // 現在のアクション
    std::function<void()> onComplete;                   // アクション完了コールバック
    bool isActive = false;                              // 処理有効フラグ

public:
    /*
     *  @brief  コンストラクタ
     */
    ActionManager() = default;
    /*
     *  @brief  デストラクタ
     */
    ~ActionManager() = default;

public:
    /*
     *  @brief      初期化処理
     *  @param[in]  JSON setJSON    セットするJSONデータ
     */
    void Initialize(JSON setJSON);
    /*
     *  @brief      更新処理
     */
    void Update(Engine& engine, float deltaTime);
    /*
     *  @brief      描画処理
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
     *  @brief      行動クラスの取得
     *  @param[in]  ActionType type
     *  @return     std::shared_ptr<DayActionBase>
     */
    std::shared_ptr<DayActionBase> GetAction(GameEnum::ActionType type, Engine& engine);
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