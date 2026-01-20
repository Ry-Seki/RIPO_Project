/*
 *	@file	MenuSystem.cpp
 *	@author	Seki
 */

#include "MenuSystem.h"
#include "../MenuManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"

/*
 *	@brief	初期化処理
 */
void MenuSystem::Initialize() {
}
/*
 *	@brief	メニューを開く
 */
void MenuSystem::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::InkSpread, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void MenuSystem::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_RETURN)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this]() {
				MenuManager::GetInstance().CloseTopMenu();
			});
		}
	}
}
/*
 *	@brief	描画処理
 */
void MenuSystem::Render() {
	DrawFormatString(50, 70, GetColor(255, 255, 255), "System");
	DrawFormatString(300, 450, GetColor(255, 255, 255), "Return->EnterKey");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSystem::Close() {
	MenuBase::Close();
}
