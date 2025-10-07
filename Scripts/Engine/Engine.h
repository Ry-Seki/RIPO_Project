/*
 *	@file	Engine.h
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "GameObject.h"
#include "VecMath.h"
#include "Scene/Scene.h"

#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>

class Engine {
private:
	GameObjectList gameObjects;		// ゲームオブジェクトの一元管理
	bool dxlibInitialized = false;
	bool effekseerInitialized = false;
	bool initialized = false;

	ScenePtr currentScene;
	ScenePtr nextScene;

public:
	Engine() = default;
	~Engine() = default;

	int Run();

private:
	int Initialize();
	void Teardown();
	void Update();
	void Render();

	void UpdateGameObjects(float deltaTime);
	void RemoveGameObjects();

public:
	/*
	 *	ゲームオブジェクトを生成する
	 *  @param name
	 *  @param position
	 *  @param rotation
	 */
	template<class T>
	std::shared_ptr<T> Create(const std::string& name,
							  const Vector3& position = { 0.0f, 0.0f, 0.0f },
							  const Vector3& rotation = { 0.0f, 0.0f, 0.0f }) {
		// オブジェクトのポインタを生成
		std::shared_ptr<T> object = std::make_shared<T>();
		object->engine = this;
		object->name = name;
		object->position = position;
		object->rotation = rotation;
		// 一元管理に追加
		gameObjects.push_back(object);
		return object;
	}
	// すべてのゲームオブジェクトを削除する
	void ClearGameObjects();

	// 次のシーンを設定する
	void SetNextScene(ScenePtr scene) { nextScene = scene; }

	// シーンの切り替え
	void ChangeScene();
};
#endif // !_ENGINE_H_


