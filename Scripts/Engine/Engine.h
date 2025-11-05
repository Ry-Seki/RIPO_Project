/*
 *  @file   Engine.h
 *  @author Seki
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "GameObject.h"
#include "Scene/Scene.h"
#include "VecMath.h"
#include "Fade/FadeBase.h"
#include "Load/LoadManager.h"

#include <memory>
#include <vector>
#include <string>
#include <functional>

/*
 *  ゲームの基盤クラス
 */
class Engine {
private:
    ScenePtr currentScene;                  // 現在のシーン
    ScenePtr nextScene;                     // 次のシーン

    bool dxlibInitialized = false;          // DxLibの初期化フラグ
    bool effekseerInitialized = false;      // Effekseerの初期化フラグ
    bool initialized = false;               // 初期化フラグ

public:
    /*
     *  コンストラクタ
     */
    Engine() = default;
    /*
     *  デストラクタ
     */
    ~Engine() = default;
public:
    /*
     *  メインゲーム
     */
    int Run();

private:
    /*
     *  初期化処理
     */
    int Initialize();
    /*
     *  破棄処理
     */
    void Teardown();
    /*
     *  更新処理
     */
    void Update();
    /*
     *  描画処理
     */
    void Render();

public:
    /*
     *  シーンの変更
     */
    void ChangeScene();
    /*
     *  フェード呼び出し処理
     *  param[in]           const FadeBasePtr& setFade          セットするフェード
     *  param[in]           std::function<void()> onComplete    コールバック
     */
    void StartSceneFade(const FadeBasePtr& setFade, std::function<void()> onComplete = nullptr);
    /*
     *  フェードアウト → 中間処理 → フェードイン（黒フェード）
     *  @param[in]          float fadeOutTime                   フェードアウト時間
     *  @param[in]          float fadeInTime                    フェードイン時間
     *  @param[in]          std::function<void()> onMidPoint    コールバック
     */
    void StartFadeOutIn(float fadeOutTime, float fadeInTime, std::function<void()> onMidPoint);

    /*
     *  GameObject生成
     *  param[in]           const std::string& name     オブジェクトの名前
     *  param[in]           const Vector3& position     オブジェクトの位置
     *  param[in]           const Vector3& rotation     オブジェクトの回転
     */
    template<class T>
    std::shared_ptr<T> Create(const std::string& name = "",
                              const Vector3& position = Vector3::zero,
                              const Vector3& rotation = Vector3::zero) {
        auto obj = std::make_shared<T>();
        obj->engine = this;
        obj->name = name;
        obj->position = position;
        obj->rotation = rotation;

        return obj;
    }

public:
    /*
     *  現在のシーンの取得
     */
    inline ScenePtr GetCurrentScene() const { return currentScene; }
    /*
     *  次のシーンの設定
     */
    inline void SetNextScene(ScenePtr setScene) { nextScene = setScene; }
    /*
     *  SceneのAddGameObjectの呼び出し
     *  @param[in]  const GameObjectPtr& gameObject
     */
    inline void AddGameObject(const GameObjectPtr& gameObject) { if (currentScene) currentScene->AddGameObject(gameObject); }
};
#endif // !_ENGINE_H_

