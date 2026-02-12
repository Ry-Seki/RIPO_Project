/*
 *	@file	Engine.cpp
 *	@author	Seki
 */

#include "Engine.h"
#include "Time.h"
#include "GameConst.h"
#include "Scene/Scene.h"
#include "Scene/StandbyScene.h"
#include "Scene/TitleScene.h"
#include "Scene/MainGameScene.h"
#include "Scene/ResultScene.h"
#include "Scene/DebugScene.h"
#include "Fade/FadeFactory.h"
#include "Fade/FadeManager.h"
#include "Audio/AudioManager.h"
#include "Load/LoadManager.h"
#include "Menu/MenuManager.h"
#include "Save/SaveDataManager.h"
#include "System/Settings/SettingsManager.h"
#include "../Data/DxLibResourcesManager.h"
#include "Input/InputManager.h"

#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <iostream>

 /*
  *	@brief		初期化処理
  */
int Engine::Initialize() {
	dxlibInitialized = false;
	effekseerInitialized = false;
	initialized = false;
	isGameEnd = false;

#pragma region DxLibの初期化処理(理解するまでは触らない)
	SetOutApplicationLogValidFlag(FALSE);
	//ウィンドウのサイズを変更する
	SetGraphMode(GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, 32, 0);
	//起動時のウィンドウのモードの設定
	ChangeWindowMode(TRUE);			// true : Windowモード, false : FullScreen
	//背景色の設定
	SetBackgroundColor(196, 196, 196);

	//DirectXを仕様宣言　理由：Effekseerを使用するために必ず設定する -> DirectX9, 11
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//DxLibの初期化
	if (DxLib_Init() == -1)
		return 0;

	dxlibInitialized = true;
	//Effekseerの初期化
	//Effecseer_Init(引数) 引数は最大のパーティクル量
	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 0;
	}
	effekseerInitialized = true;
	//描画する先を設定する　裏画面に変更する
	SetDrawScreen(DX_SCREEN_BACK);
	//図形描画のZバッファの有効化
	{
		//Zバッファを使用するかどうか	デフォルト : FALSE
		SetUseZBuffer3D(TRUE);
		//Zバッファに書き込みを行うか	デフォルト : FALSE
		SetWriteZBuffer3D(TRUE);
	}
	//ライティング
	{
		//ライトの計算をするかどうか	デフォルト : TRUE
		SetUseLighting(TRUE);
		//標準ライトを使用するかどうか	デフォルト : TRUE
		SetLightEnable(TRUE);
		//グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(0.3f, 0.3f, 0.3f, 1.0f));	//ライトの計算でα値は使わない	色がfloat型の場合、0.0f～1.0fの範囲

		//反射光の設定		Diffuse
		SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		//鏡面反射光の設定	Specular
		SetLightSpcColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
		//環境光の設定		Ambient
		SetLightAmbColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	}
#pragma endregion

	// タイムクラスの初期化
	Time::Init();
	// セーブ管理クラスの初期化
	SaveDataManager::GetInstance().Initialize();
	// 音量管理クラスの初期化
	AudioManager::GetInstance().Initialize();
	// 設定管理クラスの初期化
	SettingsManager::GetInstance().Initialize();
	// 入力管理クラスの初期化
	InputManager::GetInstance().Initialize();
	// メニュークラスの初期化
	MenuManager::GetInstance().Initialize(*this);
	// 初期化フラグの変更
	initialized = true;
	SetMouseDispFlag(TRUE);
	return 0;
}
/*
 *	@brief		破棄処理
 */
void Engine::Teardown() {
	if (initialized) {
		initialized = false;
		DxLibResourcesManager::GetInstance().Teardown();
		currentScene->Finalize(*this);
	}
	if (effekseerInitialized) { Effkseer_End(); effekseerInitialized = false; }
	if (dxlibInitialized) { DxLib_End(); dxlibInitialized = false; }
}
/*
 *	@brief		メインループ
 */
int Engine::Run() {
	if (Initialize() != 0) { Teardown(); return 1; }

	SetNextScene(std::make_shared<StandbyScene>());
	ChangeScene();

	while (!isGameEnd && ProcessMessage() != -1) {
		// 現在の時間の取得
		auto frameStart = std::chrono::high_resolution_clock::now();

		Effekseer_Sync3DSetting();
		Update();
		Render();
		// 経過時間に応じて待機
		auto frameEnd = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = frameEnd - frameStart;

		float waitTime = GameConst::frameTime - elapsed.count();
		if (waitTime > 0) {
			WaitTimer(static_cast<int>(waitTime * 1000));
		}
	}
	Teardown();
	return 0;
}
/*
 *	@brief		更新処理
 */
void Engine::Update() {
	Time::Update();

	// フェードは TimeScale 非依存で更新
	FadeManager::GetInstance().Update(Time::unscaledDeltaTime);

	// ロードフラグの取得
	bool isLoading = LoadManager::GetInstance().IsLoading();
	// ロード更新処理
	LoadManager::GetInstance().Update(Time::unscaledDeltaTime);

	// フェードモードを確認
	bool isFadeStop = FadeManager::GetInstance().GetMode() == FadeMode::Stop;

	// 入力の更新
	InputManager::GetInstance().Update();

	// メニューの更新処理
	MenuManager::GetInstance().Update(Time::unscaledDeltaTime);

	// シーンの更新処理
	if (currentScene && !isLoading && !isFadeStop) currentScene->Update(*this, Time::deltaTime);

	// 音源の更新
	AudioManager::GetInstance().Update();

	// シーンの切り替え
	ChangeScene();
}
/*
 *	@brief		描画処理
 */
void Engine::Render() {
	ClearDrawScreen();

	// シーンの描画
	if (currentScene) currentScene->Render();

	// ロードフラグの取得
	bool isLoading = LoadManager::GetInstance().IsLoading();
	// ロード描画処理
	if (isLoading) LoadManager::GetInstance().Render();

	// メニュー描画処理
	MenuManager::GetInstance().Render();

	// フェード描画
	FadeManager::GetInstance().Render();

	// ここで FPS 表示
#if _DEBUG
	DrawFormatString(10, 10, GetColor(255, 255, 255), "FPS: %.1f", Time::fps);
#endif
	ScreenFlip();
}
/*
 *	@brief		シーンの変更
 */
void Engine::ChangeScene() {
	if (nextScene) {
		if (currentScene) currentScene->Finalize(*this);
		currentScene = nextScene;
		nextScene.reset();
		if (currentScene) currentScene->Initialize(*this);
		// if(currentScene) currentScene->Setup(*this);
	}
}
/*
 *  @brief      SceneのAddGameObjectの呼び出し
 *  @param[in]  const GameObjectPtr& gameObject
 */
void Engine::AddGameObject(const GameObjectPtr& gameObject) {
	if (currentScene) currentScene->AddGameObject(gameObject);
}
/*
 *	@brief		フェードの呼び出し処理
 */
void Engine::StartSceneFade(const FadeBasePtr& setFade, std::function<void()> onComplete) {
	if (!setFade) return;
	FadeManager::GetInstance().StartFade(setFade, [this, onComplete]() {
		ChangeScene();
		if (onComplete) onComplete();
	});
}
/*
 *	@brief		フェードアウト・インコールバック付き同時呼び出し
 */
void Engine::StartFadeOutIn(float fadeOutTime, float fadeInTime, std::function<void()> onMidPoint) {
	// フェードアウト開始
	auto fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, fadeOutTime, FadeDirection::Out, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeOut, [this, fadeInTime, onMidPoint]() {
		// フェードアウト完了 → 中間処理（例：日付更新）
		if (onMidPoint) onMidPoint();

		// フェードイン開始は必ず新しい Fade または Reset
		auto fadeIn = FadeFactory::CreateFade(FadeType::InkSpread, fadeInTime, FadeDirection::In, FadeMode::NonStop);
		fadeIn->Reset(FadeDirection::In);
		FadeManager::GetInstance().StartFade(fadeIn, nullptr);
	});
}