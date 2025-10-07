/*
 *  @file   Engine.cpp
 */

#include "Engine.h"
#include "Time.h"
#include "Scheduler.h"
#include "ModelRenderer.h"
#include "SpriteRenderer.h"
#include "Scene/TitleScene.h"
#include <iostream>

/*
 *  初期化処理
 */
int Engine::Initialize() {
    // フラグ初期化
    dxlibInitialized = false;
    effekseerInitialized = false;
    initialized = false;

#pragma region DxLibの初期化処理
    SetOutApplicationLogValidFlag(FALSE);
    SetGraphMode(800, 600, 32, 0);
    ChangeWindowMode(TRUE);
    SetBackgroundColor(196, 196, 196);
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // DxLib初期化
    if (DxLib_Init() == -1) return 1;
    dxlibInitialized = true;

    // Effekseer初期化
    if (Effekseer_Init(8000) == -1) return 1;
    effekseerInitialized = true;

    // 描画先を裏画面に設定
    SetDrawScreen(DX_SCREEN_BACK);

    // Zバッファ
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // ライティング
    SetUseLighting(TRUE);
    SetLightEnable(TRUE);
    SetGlobalAmbientLight(GetColorF(0.3f, 0.3f, 0.3f, 1.0f));
    SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
    SetLightSpcColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
    SetLightAmbColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
#pragma endregion

    // Timeクラス初期化
    Time::Init();
    initialized = true;
    return 0;
}
/*
 *  破棄処理
 */
void Engine::Teardown() {
    // Effekseer終了
    if (effekseerInitialized) {
        Effkseer_End();
        effekseerInitialized = false;
    }

    // DxLib終了
    if (dxlibInitialized) {
        DxLib_End();
        dxlibInitialized = false;
    }

    initialized = false;
}
/*
 *  メインループ
 */
int Engine::Run() {
    if (Initialize() != 0) {
        Teardown();
        return 1;
    }

    // 初期シーンを設定して即切り替え
    SetNextScene(std::make_shared<TitleScene>());
    ChangeScene(); // currentScene が初期化される

    while (ProcessMessage() != -1) {
        Effekseer_Sync3DSetting();

        Update();
        Render();
        RemoveGameObjects();
    }
    Teardown();
    return 0;
}/*
 *  更新処理
 */
void Engine::Update() {
    Time::Update();
    float deltaTime = Time::deltaTime;

    // 非同期タスクの更新
    Scheduler::Instance().Update();

    // Scene更新
    if (currentScene) currentScene->Update(*this, deltaTime);

    // シーンの切り替え
    ChangeScene();

    UpdateGameObjects(deltaTime);
}
/*
 *  描画処理
 */
void Engine::Render() {
    ClearDrawScreen();

    // 3D描画
    for (auto& obj : gameObjects) {
        if (obj->IsDestroyed()) continue;

        for (auto& model : obj->GetComponents<ModelRenderer>()) {
            model->Render3D();
        }
    }

    // 2D描画
    for (auto& obj : gameObjects) {
        if (obj->IsDestroyed()) continue;

        for (auto& sprite : obj->GetComponents<SpriteRenderer>()) {
            sprite->Render2D();
        }
    }
    if (currentScene) currentScene->Render();

    ScreenFlip();
}
/*
 *  オブジェクトの更新処理
 */
void Engine::UpdateGameObjects(float deltaTime) {
    for (auto& obj : gameObjects) {
        if (!obj->IsDestroyed()) {
            obj->Start();
            obj->Update(deltaTime);
        }
    }
}
/*
 *  オブジェクトの破棄処理
 */
void Engine::RemoveGameObjects() {
    const auto itr = std::stable_partition(
        gameObjects.begin(), gameObjects.end(),
        [](const auto& obj) { return !obj->IsDestroyed(); });

    gameObjects.erase(itr, gameObjects.end());
}
/*
 *  全てのオブジェクトの破棄
 */
void Engine::ClearGameObjects() {
    for (auto& obj : gameObjects) {
        obj->OnDestroy();
    }
    gameObjects.clear();
}

void Engine::ChangeScene() {
    // Scene切り替え
    if (nextScene) {
        if (currentScene) currentScene->Finalize(*this);
        currentScene = nextScene;
        nextScene.reset();
        if (currentScene) currentScene->Initialize(*this);
    }
}