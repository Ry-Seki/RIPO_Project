/*
 *	@file	SceneState_Tutorial
 *	@author	Seki
 */

#ifndef _SCENE_STATE_TUTORIAL_H_
#define _SCENE_STATE_TUTORIAL_H_

#include "../SceneStateBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	チュートリアルシーンステート
 */
class SceneState_Tutorial : public SceneStateBase {

public:
    /*
     *  @brief  デストラクタ
     */
    ~SceneState_Tutorial() override = default;

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

#endif // !_SCENE_STATE_TUTORIAL_H_