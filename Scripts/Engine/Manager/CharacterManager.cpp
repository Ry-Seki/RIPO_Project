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

template <typename T>
CharacterBasePtr CharacterManager::CreateCharacter(
	int setID,
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	// 未使用状態のオブジェクト取得
	characterObject = GameObjectManager::GetInstance().GetUnuseObject();
	// キャラクター生成
	CharacterBasePtr createCharacter = characterObject->AddComponent<T>();
	// ID設定
	createCharacter->SetID(setID);
	// データのセット
	characterObject->SetObjectData(name, position, rotation);
	// キャラクターを返す
	return createCharacter;
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
		// カメラのターゲットに追加
		CameraManager::GetInstance().SetTarget(characterObject);
		// リストの空きに生成
		createCharacterList[i] = CreateCharacter<PlayerComponent>(i, name, position, rotation);
		return;
	}
	// カメラのターゲットに追加
	CameraManager::GetInstance().SetTarget(characterObject);
	// 空きが無かったら一番後ろに生成
	createCharacterList.push_back(CreateCharacter<PlayerComponent>(0, name, position, rotation));
}

/*
 *	ID指定のキャラクター削除
 */
void CharacterManager::RemoveCharacter(int characterID) {
	// リストから削除
	createCharacterList[characterID] = nullptr;
	// オブジェクトのリセット
	GameObjectManager::GetInstance().ResetObject(characterObject);
}

/*
 *	ID指定のキャラクター取得
 */
CharacterBasePtr CharacterManager::GetCharacter(int characterID) {
	return createCharacterList[characterID];
}
