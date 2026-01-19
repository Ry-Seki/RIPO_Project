/*
 *	@file	InAction_Shop.cpp
 *	@author	Seki
 */

#include "InAction_Shop.h"
#include "../GameState_InAction.h"
#include "../../GameStateMachine.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"

/*
 *	@brief	初期化処理
 */
void InAction_Shop::Initialize(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void InAction_Shop::Setup() {
	isStart = false;
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});

}
/*
 *	@brief	更新処理
 */
void InAction_Shop::Update(float deltaTime) {
	if (isStart) {
		isStart = false;
		FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::Out, FadeMode::Stop);
		FadeManager::GetInstance().StartFade(fadeOut, [this]() {
			owner->GetOwner()->ChageState(GameEnum::GameState::ResultAction);
		});
	}
}
/*
 *	@brief	描画処理
 */
void InAction_Shop::Render() {
}
/*
 *	@brief	片付け処理
 */
void InAction_Shop::Teardown() {
}
