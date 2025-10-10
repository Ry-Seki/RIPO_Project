#include "Engine.h"
#include "Time.h"
#include "Scene/TitleScene.h"
#include "Fade/FadeFactory.h"
#include "Fade/FadeManager.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <iostream>

int Engine::Initialize() {
    dxlibInitialized = false;
    effekseerInitialized = false;
    initialized = false;

#pragma region DxLibの初期化処理(理解するまでは触らない)
	SetOutApplicationLogValidFlag(FALSE);
	//ウィンドウのサイズを変更する
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0);
	//起動時のウィンドウのモードの設定
	ChangeWindowMode(TRUE);		// true : Windowモード, false : FullScreen
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

    Time::Init();

    initialized = true;
    return 0;
}

void Engine::Teardown() {
	if (effekseerInitialized) { Effkseer_End(); effekseerInitialized = false; }
	if (dxlibInitialized) { DxLib_End(); dxlibInitialized = false; }
	initialized = false;
}

int Engine::Run() {
	if (Initialize() != 0) { Teardown(); return 1; }

	SetNextScene(std::make_shared<TitleScene>());
	ChangeScene();

	while (ProcessMessage() != -1) {
		Effekseer_Sync3DSetting();
		Update();
		Render();
	}

	Teardown();
	return 0;
}

void Engine::Update() {
	Time::Update();

	// フェードは TimeScale 非依存で更新
	FadeManager::GetInstance().Update(Time::unscaledDeltaTime);

	// フェードモードを確認
	bool isFadeStop = FadeManager::GetInstance().GetMode() == FadeMode::Stop;

	// シーンの更新
	if (!isFadeStop && currentScene) currentScene->Update(*this, Time::deltaTime);

	// シーンの切り替え
	ChangeScene();
}

void Engine::Render() {
	ClearDrawScreen();

	// シーンの描画
	if (currentScene) currentScene->Render();

	// フェード描画
	FadeManager::GetInstance().Render();
	ScreenFlip();
}

void Engine::ChangeScene() {
	if (nextScene) {
		if (currentScene) currentScene->Finalize(*this);
		currentScene = nextScene;
		nextScene.reset();
		if (currentScene) currentScene->Initialize(*this);
	}
}

void Engine::StartSceneFade(const FadeBasePtr& setFade, std::function<void()> onComplete) {
	if (!setFade) return;
	FadeManager::GetInstance().StartFade(setFade, [this, onComplete]() {
		ChangeScene();
		if (onComplete) onComplete();
	});
}
void Engine::StartFadeOutIn(float fadeOutTime, float fadeInTime, std::function<void()> onMidPoint) {
	// フェードアウト開始
	auto fadeOut = CreateFade(FadeType::Black, fadeOutTime, FadeDirection::Out, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeOut, [this, fadeInTime, onMidPoint]() {
		// フェードアウト完了 → 中間処理（例：日付更新）
		if (onMidPoint) onMidPoint();

		// フェードイン開始は必ず新しい Fade または Reset
		auto fadeIn = CreateFade(FadeType::Black, fadeInTime, FadeDirection::In, FadeMode::NonStop);
		fadeIn->Reset(FadeDirection::In);
		FadeManager::GetInstance().StartFade(fadeIn, nullptr);
	});
}
