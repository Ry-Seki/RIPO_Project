/*
 *	@file	SceneStateMachine.cpp
 */

#include "SceneStateMachine.h"

/*
 *	@brief	初期化処理
 */
void SceneStateMachine::Initialize(Engine& engine) {
	// シーンステートを要素分、あらかじめ確保しておく
	sceneStateList.resize(static_cast<int>(GameEnum::SceneType::Max));
	// 各シーンステートを生成する 
	// ※GameEnum::SceneTypeの順番変更が変更されても大丈夫なように、手動で登録(他も同様)
}
/*
 *	@brief	準備前処理
 */
void SceneStateMachine::Setup() {
}
/*
 *	@brief	更新処理
 */
void SceneStateMachine::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void SceneStateMachine::Render() {
}
/*
 *	@brief	片付け処理
 */
void SceneStateMachine::Teardown() {
}
