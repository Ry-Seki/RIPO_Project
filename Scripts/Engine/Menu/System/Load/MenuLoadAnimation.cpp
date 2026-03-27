/*
 *	@file	MenuLoadAnimation.cpp
 *	@author	Seki
 */

#include "MenuLoadAnimation.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Fade/FadeManager.h"
#include "../../../Fade/FadeFactory.h"
#include "../../../Input/InputUtility.h"
#include "../../../Engine.h"
#include "../../../../Data/UI/MenuInfo.h"
#include "../../MenuResourcesFactory.h"
#include "../../MenuManager.h"

namespace {
    constexpr const char* _MENU_RESOURCES_PATH = "";
}
/*
 *	@brief	初期化処理
 */
void MenuLoadAnimation::Initialize(Engine& engine) {
    auto& load = LoadManager::GetInstance();
    auto menuJSON = load.LoadResource<LoadJSON>(_MENU_RESOURCES_PATH);

    load.SetOnComplete([this, menuJSON]() {
        MenuInfo result = MenuResourcesFactory::Create(menuJSON->GetData());
        spriteList = std::move(result.spriteList);
    });
}
/*
 *	@brief	メニューを開く
 */
void MenuLoadAnimation::Open() {
    MenuBase::Open();
    animTimer = 0.0f;
    for (auto& sprite : spriteList) {
        sprite->Setup();
    }
}
/*
 *	@brief	更新処理
 */
void MenuLoadAnimation::Update(Engine& engine, float unscaledDeltaTime) {
}
/*
 *	@brief	アニメーション等の更新
 */
void MenuLoadAnimation::AnimUpdate(Engine& engine, float unscaledDeltaTime) {
    animTimer += unscaledDeltaTime;

    if (animTimer < GameConst::UI_ANIM_INTERVAL) return;
    animTimer = 0;

    for (auto& sprite : spriteList) {
        int frameCount = sprite->GetFrameCount();
        if (frameCount <= 1) continue;

        animFrame = (animFrame + 1) % frameCount;
        sprite->SetFrameIndex(animFrame);
    }
}
/*
 *	@brief	描画処理
 */
void MenuLoadAnimation::Render() {
    for (auto& sprite : spriteList) {
        if (!sprite->IsVisible()) continue;
        sprite->Render();
    }
}
/*
 *	@brief	メニューを閉じる
 */
void MenuLoadAnimation::Close(Engine& engine) {
    MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuLoadAnimation::Resume() {
}
