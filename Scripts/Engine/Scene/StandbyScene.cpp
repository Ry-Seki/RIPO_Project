/*
 *	@file	StandbyScene.cpp
 *	@author	Seki
 */

#include "StandbyScene.h"
#include "TitleScene.h"
#include "../Engine.h"
#include "../Fade/FadeManager.h"
#include "../Fade/FadeFactory.h"
#include "../Menu/MenuManager.h"
#include "../Menu/System/MenuLoadMode.h"
#include "../Menu/System/MenuSaveMode.h"
#include "../Menu/System/MenuSettings.h"

/*
 *	コンストラクタ
 */
StandbyScene::StandbyScene() {
}
/*
 *	デストラクタ
 */
StandbyScene::~StandbyScene() {
}
/*
 *	初期化処理
 */
void StandbyScene::Initialize(Engine& engine) {
	auto& menu = MenuManager::GetInstance();
	menu.GetMenu<MenuLoadMode>();
	menu.GetMenu<MenuSaveMode>();
	menu.GetMenu<MenuSettings>();
	auto fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeOut, [&engine]() {
		engine.SetNextScene(std::make_shared<TitleScene>());
	});
}
/*
 *	ロード済みデータのセット
 */
void StandbyScene::SetupData(Engine& engine) {
	auto fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeOut, [&engine]() {
		engine.SetNextScene(std::make_shared<TitleScene>());
	});
}
/*
 *	更新処理
 */
void StandbyScene::Update(Engine& engine, float deltaTime) {
}
/*
 *	描画処理
 */
void StandbyScene::Render() {
}
