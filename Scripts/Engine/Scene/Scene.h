/*
 *  @file   Scene.h
 *  @author Seki
 */

#ifndef _SCENE_H_
#define _SCENE_H_

#include "../GameObject.h"
#include <vector>
#include <memory>

// 前方宣言
class Engine;

/*
 *  シーンの基底クラス
 */
class Scene {
protected:
    std::vector<GameObjectPtr> gameObjects;     // ゲームオブジェクト配列

public:
    /*
     *  コンストラクタ
     */
    Scene() = default;
    /*
     *  デストラクタ
     */
    virtual ~Scene() = default;

public:
    /*
     *  初期化処理
     */
    virtual void Initialize(Engine& engine);
    /*
     *  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime);
    /*
     *  描画処理
     */
    virtual void Render();
    /*
     *  破棄処理
     */
    virtual void Finalize(Engine& engine);

public:
    /*
     *  ゲームオブジェクトの追加
     *  param[in]           const GameObjectPtr& obj    ゲームオブジェクトのポインタ
     */
    void AddGameObject(const GameObjectPtr& obj);
    /*
     *  ゲームオブジェクトの破棄
     */
    void RemoveDestroyedObjects();
};
// 別名定義
using ScenePtr = std::shared_ptr<Scene>;

#endif // !_SCENE_H_