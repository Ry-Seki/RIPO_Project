/*
 *	@file	SceneState_MainGame.h
 *	@author	Seki
 */

#ifndef _SCENE_STATE_MAINGAME_H_
#define _SCENE_STATE_MAINGAME_H_

#include "../SceneStateBase.h"

/*
 *	@brief	メインゲームシーンステート
 */
class SceneState_MainGame : public SceneStateBase {

public:
    /*
     *  @brief  デストラクタ
     */
    ~SceneState_MainGame() override = default;

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
     *  @brief  更新処理
     */
    void Update(Engine& engine, float deltaTime) override;
    /*
     *  @brief  描画処理
     */
    void Render() override;
    /*
     *  @biref  片付け処理
     */
    void Teardown(Engine& engine) override;

};

#endif // !_SCENE_STATE_MAINGAME_H_
