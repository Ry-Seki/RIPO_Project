/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "CharacterManager.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "../Component/Character/ArmActionComponent.h"
#include "../Component/ModelRenderer.h"

CharacterManager::CharacterManager() 
	: engine(nullptr) {
}

template <typename T>
CharacterBasePtr CharacterManager::CreateCharacter(
	int setID,
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax,
	GameObjectPtr& characterObject) {
	// 未使用状態のオブジェクト取得
	characterObject = GameObjectManager::GetInstance().GetUnuseObject();
	// キャラクター生成
	CharacterBasePtr createCharacter = characterObject->AddComponent<T>();
	// コライダー生成
	AABBColliderPtr collider = characterObject->AddComponent<AABBCollider>();
	// モデルコンポーネントの追加
	characterObject->AddComponent<ModelRenderer>();
	collider->aabb = { AABBMin, AABBMax };
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
	// はじめに一定数生成
	createCharacterList.reserve(CREATE_CHARACTER_COUNT);
	createCharacterObjectList.reserve(CREATE_CHARACTER_COUNT);
	for (size_t i = 0; i < CREATE_CHARACTER_COUNT; i++) {
		// 空の要素を生成
		createCharacterList.push_back(nullptr);
		createCharacterObjectList.push_back(nullptr);
	}
}

/*
 *	プレイヤー生成
 */
void CharacterManager::GeneratePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	GameObjectPtr player;
	// リストの要素の数
	size_t characterListCount = createCharacterList.size();
	// 生成キャラクターリストの空きをチェック
	for (size_t i = 0; i < characterListCount; i++) {
		if (createCharacterList[i] != nullptr) continue;
		// リストの空きに生成
		createCharacterList[i] = CreateCharacter<PlayerComponent>(i, name, position, rotation, AABBMin, AABBMax, player);
		// オブジェクトのリストも保存
		createCharacterObjectList[i] = player;
		// ウデアクションコンポーネント追加
		player->AddComponent<ArmActionComponent>();
		// カメラのターゲットに追加
		CameraManager::GetInstance().SetTarget(player);
		// シーンが持つゲームオブジェクト配列に追加
		engine->AddGameObject(player);
		return;
	}
	// 空きが無かったら一番後ろに生成
	createCharacterList.push_back(CreateCharacter<PlayerComponent>(0, name, position, rotation, AABBMin, AABBMax, player));
	// オブジェクトのリストも保存
	createCharacterObjectList.push_back(player);
	// カメラのターゲットに追加
	CameraManager::GetInstance().SetTarget(player);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(player);
}

/*
 *	エネミー生成
 */
void CharacterManager::GenerateEnemy(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	GameObjectPtr enemy;
	// リストの要素の数
	size_t characterListCount = createCharacterList.size();
	// 生成キャラクターリストの空きをチェック
	for (size_t i = 0; i < characterListCount; i++) {
		if (createCharacterList[i] != nullptr) continue;
		// リストの空きに生成
		createCharacterList[i] = CreateCharacter<EnemyComponent>(i, name, position, rotation, AABBMin, AABBMax, enemy);
		// オブジェクトリストにも保存
		createCharacterObjectList[i] = enemy;
		// シーンが持つゲームオブジェクト配列に追加
		engine->AddGameObject(enemy);
		return;
	}
	// 空きが無かったら一番後ろに生成
	createCharacterList.push_back(CreateCharacter<EnemyComponent>(0, name, position, rotation, AABBMin, AABBMax, enemy));
	// オブジェクトリストにも保存
	createCharacterObjectList.push_back(enemy);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(enemy);
}

/*
 *	ID指定のキャラクター削除
 */
void CharacterManager::RemoveCharacter(int characterID) {
	GameObjectPtr destoryObject = createCharacterObjectList[characterID];
	// オブジェクトのリセット
	GameObjectManager::GetInstance().ResetObject(destoryObject);
	// リストから削除
	createCharacterList[characterID] = nullptr;
	// オブジェクトリストから削除
	createCharacterObjectList[characterID] = nullptr;
}
/*
 *	全てのキャラクター削除処理
 *  @author	Seki
 */
void CharacterManager::RemoveAllCharacter() {
	for (int i = 0, max = createCharacterObjectList.size(); i < max; i++) {
		RemoveCharacter(i);
	}
}

/*
 *	ID指定のキャラクター取得
 */
CharacterBasePtr CharacterManager::GetCharacter(int characterID) const{
	return createCharacterList[characterID];
}
/*
 *	キャラクターにモデルハンドルをセット
 *	@param[in]	GameObject* gameObject	セットするモデル
 *  @param[in]	const int modelHandle		モデルハンドル
 *  @author		Seki
 */
void CharacterManager::SetModelHandle(GameObject* gameObject, const int modelHandle) {
	if (!gameObject) return;
	auto modelRenderer = gameObject->GetComponent<ModelRenderer>();
	if (!modelRenderer) return;

	modelRenderer->SetModelHandle(modelHandle);
}
/*
 *	キャラクターのオーナーオブジェクトの取得
 *  @param[in]	const CharacterBasePtr& setCharacter
 *  @return		GameObject*
 *  @author		Seki
 */
GameObject* CharacterManager::GetCharacterOwner(const CharacterBasePtr& setCharacter) const {
	if (!setCharacter) return nullptr;
	GameObject* owner = setCharacter->GetOwner();
	if (!owner) return nullptr;

	return owner;
}
