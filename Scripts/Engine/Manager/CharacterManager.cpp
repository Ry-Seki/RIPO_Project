/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "CharacterManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"

CharacterManager::CharacterManager() 
	: engine(nullptr) {
}

CharacterBasePtr CharacterManager::CreatePlayer(
	int setID,
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// 未使用状態のオブジェクト取得
	playerObject = GameObjectManager::GetInstance().GetUnuseObject();
	// プレイヤーキャラクター生成
	CharacterBasePtr playerCharacter = playerObject->AddComponent<PlayerComponent>();
	// ID設定
	playerCharacter->SetID(setID);
	// データのセット
	playerObject->SetObjectData(name, position, rotation);
	// カメラのターゲットに追加
	CameraManager::GetInstance().SetTarget(playerObject);
	// プレイヤーキャラクターを返す
	return playerCharacter;
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
void CharacterManager::GeneratePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// リストの要素の数
	int characterListCount = static_cast<int>(createCharacterList.size());
	// 生成キャラクターリストの空きをチェック
	for (int i = 0; i < characterListCount; i++) {
		if (createCharacterList[i] != nullptr) continue;
		// リストの空きに生成
		createCharacterList[i] = CreatePlayer(i, name, position, rotation);
		return;
	}
	// 空きが無かったら一番後ろに生成
	createCharacterList.push_back(CreatePlayer(0, name, position, rotation));
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
