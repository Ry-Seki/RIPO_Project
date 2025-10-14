/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "CharacterManager.h"
#include "GameObjectManager.h"

CharacterManager::CharacterManager() 
	: engine(nullptr) {
}

/*
 *	初期化処理
 */
void CharacterManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
}

/*
 *	プレイヤー生成
 */
void CharacterManager::CreatePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	int characterListCount = static_cast<int>(createCharacterList.size());
	for (int i = 0; i < characterListCount; i++) {
		if (createCharacterList[i] != nullptr) continue;
		// 未使用状態のオブジェクト取得
		playerObject = GameObjectManager::GetInstance().GetUnuseObject();
		// プレイヤーキャラクター生成
		CharacterBasePtr playerCharacter = playerObject->AddComponent<PlayerComponent>();
		// ID設定
		playerCharacter->SetID(i);
		// データのセット
		playerObject->SetObjectData(name, position, rotation);
		// リストに追加
		createCharacterList[i] = playerCharacter;
	}

}

/*
 *	ID指定のキャラクター削除
 */
void CharacterManager::RemoveCharacter(int characterID) {
	// リストから削除
	createCharacterList[characterID] = nullptr;
	// オブジェクトのリセット
	GameObjectManager::GetInstance().ResetObject(playerObject);
}

/*
 *	ID指定のキャラクター取得
 */
CharacterBasePtr CharacterManager::GetCharacter(int characterID) {
	return createCharacterList[characterID];
}
