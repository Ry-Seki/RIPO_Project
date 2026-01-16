/*
 *	@file	InAction_Training.cpp
 */

#include "InAction_Training.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"

/*
 *	@brief	初期化処理
 */
void InAction_Training::Initialize(Engine& engine) {
}
/*
 *	@brief	準備前処理
 */
void InAction_Training::Setup() {
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Tile, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fade);
}
/*
 *	@brief	更新処理
 */
void InAction_Training::Update(float deltaTime) {

}
/*
 *	@brief	描画処理
 */
void InAction_Training::Render() {
}
/*
 *	@brief	片付け処理
 */
void InAction_Training::Teardown() {
}
