/*
 *	@file	GameObjectManager.cpp
 *  @author	Riku
 */
#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
	: engine(nullptr) {
}

/*
 *	初期化処理
 */
void GameObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
	// はじめに一定数生成
	unuseObjectList.reserve(CREATE_OBJECT_COUNT);
	for (size_t i = 0; i < CREATE_OBJECT_COUNT; i++) {
		// 未使用オブジェクト生成
		unuseObjectList.push_back(engine->Create<GameObject>());
	}
}

/*
 *	未使用オブジェクトを渡す
 */
GameObjectPtr GameObjectManager::GetUnuseObject() {
	// 同時にリストをいじれないようにロック
	std::lock_guard<std::mutex> lock(unuseMutex);
	// リストが空なら生成して返す
	if (unuseObjectList.empty()) return engine->Create<GameObject>();
	// リストの一番後ろを渡す
	GameObjectPtr unuseObject = unuseObjectList.back();
	// 渡したらリストから削除
	unuseObjectList.pop_back();
	return unuseObject;
}

/*
 *	オブジェクトのリセット
 */
void GameObjectManager::ResetObject(GameObjectPtr resetObject) {
	// 同時にリストをいじれないようにロック
	std::lock_guard<std::mutex> lock(unuseMutex);
	// オブジェクトのリセット
	resetObject->OnDestroy();
	resetObject->ResetGameObject();
	// 未使用リストに戻る
	unuseObjectList.push_back(resetObject);
}
