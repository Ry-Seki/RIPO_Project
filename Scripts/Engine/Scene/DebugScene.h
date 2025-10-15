/*
 *	@file	DebugScene.h
 *	@author	Seki
 */

#ifndef _DEBUG_SCENE_H_
#define _DEBUG_SCENE_H_

#include "Scene.h"

class DebugScene : public Scene {
public:
    /*
     *  コンストラクタ
     */
    DebugScene() = default;
    /*
     *  デストラクタ
     */
    virtual ~DebugScene() = default;

public:
    /*
     *  初期化処理
     */
    void Initialize(Engine& engine) override;
    /*
     *  更新処理
     */
    void Update(Engine& engine, float deltaTime) override;
    /*
     *  描画処理
     */
    void Render() override;
};

#endif // !_DEBUG_SCENE_H_

