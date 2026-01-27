/*
 *  @file   MainGameScene.h
 *  @author Seki
 */

#ifndef _MAINGAMESCENE_H_
#define _MAINGAMESCENE_H_

#include "Scene.h"
#include "../GameEnum.h"
#include "GameState/GameStateMachine.h"

#include <memory>

// 前方宣言
class GameStateMachine;

/*
 *  メインゲームシーン
 */
class MainGameScene : public Scene {
private:
    std::unique_ptr<GameStateMachine> gameState;

    static constexpr const char* _TREASURE_DATA_PATH = "Data/Treasure/TreasureDataList.json";
    static constexpr const char* _ITEM_DATA_PATH = "Data/Item/ItemCatalogData.json";

    const int _END_DAY = 31;

public:
    /*
     *  @brief  コンストラクタ
     */
    MainGameScene() = default;
    /*
     *  @brief  デストラクタ
     */
    ~MainGameScene() override;

public:
    /*
     *  @brief  初期化処理
     */
    void Initialize(class Engine& engine) override;
    /*
     *  @brief  準備前処理
     */
    void Setup(Engine& engine) override;
    /*
     *  更新処理
     */
    void Update(class Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;

private:
    /*
     *  @brief  メインシーン終了処理
     */
    void EndMainGameScene(Engine& engine);

};

#endif // !_MAINGAMESCENE_H_