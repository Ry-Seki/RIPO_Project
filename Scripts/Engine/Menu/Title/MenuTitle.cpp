/*
 *	@file	MenuTitle.cpp
 *	@author	Seki
 */

#include "MenuTitle.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../MenuManager.h"
#include "MenuGameModeSelect.h"

/*
 *	@brief	初期化処理
 */
void MenuTitle::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto menuTitleHandle = load.LoadResource<LoadSprite>(_TITLE_LOGO_PATH);
	load.SetOnComplete([this, menuTitleHandle]() {
		SetupData(menuTitleHandle->GetHandle());
	});
}
/*
 *	@brief	メニューを開く処理
 */
void MenuTitle::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void MenuTitle::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.2f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this]() {
				isVisible = false;
				MenuManager::GetInstance().OpenMenu<MenuGameModeSelect>();
			});
		}
	}
}
/*
 *	@brief	描画処理
 */
void MenuTitle::Render() {
	DrawExtendGraph(150, 50, 900, 400, titleGraphHandle, TRUE);
	DrawFormatString(300, 400, GetColor(255, 255, 255), "Play->SpaceKey");
}
/*
 *	@brief	メニューを閉じる処理
 */
void MenuTitle::Close() {
	MenuBase::Close();
}
/*
 *	@brief		ロード済みデータのセット
 *  @prarm[in]	int setHandle
 */
void MenuTitle::SetupData(int setHandle) {
	titleGraphHandle = setHandle;
}
