/*
 *  @file   Engine.cpp
 */

#include "Engine.h"
#include <iostream>

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

    previousTime = static_cast<float>(GetNowCount());
    initialized = true;
    return 0;
}

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

void Engine::Update() {
    float currentTime = static_cast<float>(GetNowCount());
    deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    UpdateGameObjects(deltaTime);
}

void Engine::Render() {
    ClearDrawScreen();

    for (auto& obj : gameObjects) {
        if (!obj->IsDestroyed()) obj->Render();
    }
}

int Engine::Run() {
    if (Initialize() != 0) {
        Teardown();
        return 1;
    }
    while (ProcessMessage() != -1) {
        Effekseer_Sync3DSetting();

        Update();
        Render();
        RemoveGameObjects();

        ScreenFlip();
    }
    Teardown();
    return 0;
}

void Engine::UpdateGameObjects(float deltaTime) {
    for (auto& obj : gameObjects) {
        if (!obj->IsDestroyed()) {
            obj->Start();
            obj->Update(deltaTime);
        }
    }
}

void Engine::RemoveGameObjects() {
    const auto itr = std::stable_partition(
        gameObjects.begin(), gameObjects.end(),
        [](const auto& obj) { return !obj->IsDestroyed(); });

    gameObjects.erase(itr, gameObjects.end());
}

void Engine::ClearGameObjects() {
    for (auto& obj : gameObjects) {
        obj->OnDestroy();
    }
    gameObjects.clear();
}