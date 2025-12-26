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
    void Setup() override;
    /*
     *  更新処理
     */
    void Update(class Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;

};

#endif // !_MAINGAMESCENE_H_