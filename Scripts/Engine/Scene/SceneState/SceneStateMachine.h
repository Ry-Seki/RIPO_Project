/*
 *	@file	SceneStateMachine.h
 *	@author	Seki
 */

#ifndef _SCENE_STATE_MACHINE_H_
#define _SCENE_STATE_MACHINE_H_

#include <vector>
#include <memory>

// 前方宣言
class Engine;
class SceneStateBase;

/*
 *	@brief	シーンステート制御クラス
 */
class SceneStateMachine {
public:		// 別名定義
	using SceneStatePtr = std::shared_ptr<SceneStateBase>;

private:
	std::vector<SceneStatePtr> sceneStateList;
	SceneStatePtr currentSceneState = nullptr;

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine);
	/*
	 *	@brief	準備前処理
	 */
	void Setup();
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime);
	/*
	 *	@brief	描画処理
	 */
	void Render();
	/*
	 *	@brief	片付け処理
	 */
	void Teardown();

};

#endif // !_SCENE_STATE_MACHINE_H_