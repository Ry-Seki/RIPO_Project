/*
 *	@file	GameObjectManager.cpp
 */
#include "GameObjectManager.h"

void GameObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;

	for (int i = 0; i < CREATE_OBJECT_MAX; i++) {
		// 未使用オブジェクト生成
		GameObjectPtr playerObject = engine->Create<GameObject>();
	}
}
