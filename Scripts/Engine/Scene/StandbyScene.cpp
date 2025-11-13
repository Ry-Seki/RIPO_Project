/*
 *	@file	StandbyScene.cpp
 *	@author	Seki
 */

#include "StandbyScene.h"
#include "TitleScene.h"
#include "../Engine.h"

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
}
/*
 *	ロード済みデータのセット
 */
void StandbyScene::SetupData(Engine& engine) {

	engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
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
