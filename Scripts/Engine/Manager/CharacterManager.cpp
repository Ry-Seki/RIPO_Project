/*
 *	@file	PlayerComponent.cpp
 */
#include "CharacterManager.h"

 /*
  *	初期化処理
  */
void CharacterManager::Initialize(Engine& setEngine) {
	engine = &setEngine;

	for (int i = 0; i < CREATE_OBJECT_MAX; i++) {
		// 未使用オブジェクト生成
		GameObjectPtr playerObject = engine->Create<GameObject>(name, position, rotation);
	}

}

/*
 *	プレイヤー生成
 */
void CharacterManager::CreatePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	int objectListCount = static_cast<int>(createObjectList.size());
	for (int i = 0; i < objectListCount; i++) {
		if (createObjectList[i] != nullptr) continue;
		// プレイヤーキャラクター生成
		CharacterBasePtr playerCharacter = playerObject->AddComponent<PlayerComponent>();
		// ID設定
		playerCharacter->SetID(i);

		createObjectList[i] = playerObject;
		createCharacterList[i] = playerCharacter;
	}

}

/*
 *	ID指定のキャラクター削除
 */
void CharacterManager::RemoveCharacter(int characterID) {

	createObjectList[characterID] = nullptr;
}

/*
 *	ID指定のキャラクター取得
 */
CharacterBasePtr CharacterManager::GetCharacter(int characterID) {
	return createCharacterList[characterID];
}
