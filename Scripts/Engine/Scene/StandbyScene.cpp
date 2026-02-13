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
#include "../Load/Sprite/LoadSprite.h"
#include "../Load/LoadManager.h"
#include "../Load/Animation/LoadAnimation_ChangeBackground.h"

#include <DxLib.h>

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
	std::vector<std::shared_ptr<LoadSprite>> loadBGList;
	loadBGList.push_back(LoadManager::GetInstance().LoadResource<LoadSprite>("Res/BackGround/Trealine_LoadBackground_black1.jpg"));
	loadBGList.push_back(LoadManager::GetInstance().LoadResource<LoadSprite>("Res/BackGround/Trealine_LoadBackground_black2.jpg"));
	loadBGList.push_back(LoadManager::GetInstance().LoadResource<LoadSprite>("Res/BackGround/Trealine_LoadBackground_black3.jpg"));
	LoadManager::GetInstance().SetOnComplete([&engine, this, loadBGList]() {
		auto loadBG = std::make_shared<LoadAnimation_ChangeBackground>();
		std::vector<int> BGHandleList;
		for (int i = 0, max = loadBGList.size(); i < max; i++) {
			int handle = loadBGList[i]->GetHandle();
			BGHandleList.push_back(handle);
		}
		loadBG->SetImages(BGHandleList);
		LoadManager::GetInstance().AddAnimation(loadBG);
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [&engine]() {
			engine.SetNextScene(std::make_shared<TitleScene>());
		});
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
	DrawBox(0, 0, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
}
