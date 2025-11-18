/*
 *	@file	PlayerComponent.cpp
 *  @author	Riku
 */

#include "CharacterManager.h"
#include "CameraManager.h"
#include "../GameObject/GameObjectUtility.h"
#include "../Component/Character/ArmActionComponent.h"
#include "../Component/ModelRenderer.h"
#include "../Component/GravityComponent.h"
#include "../Component/CapsuleCollider.h"

CharacterManager::CharacterManager()
	: engine(nullptr) {}

template <typename T>
GameObjectPtr CharacterManager::CreateCharacter(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	// 未使用状態のオブジェクト取得
	GameObjectPtr characterObject = GameObjectManager::GetInstance().GetUnuseObject();
	// キャラクターコンポーネント追加
	characterObject->AddComponent<T>();
	// コライダーコンポーネント追加
	AABBColliderPtr collider = characterObject->AddComponent<AABBCollider>();
	collider->aabb = { AABBMin, AABBMax };
	// モデルコンポーネント追加
	characterObject->AddComponent<ModelRenderer>();
	// 重力コンポーネント追加
	characterObject->AddComponent<GravityComponent>();
	// データのセット
	characterObject->SetObjectData(name, position, rotation);
	// キャラクターを返す
	return characterObject;
}

template<typename T>
GameObjectPtr CharacterManager::CreateCharacter(const std::string& name, const Vector3& position, const Vector3& rotation, const Vector3& capsuleStart, const Vector3& capsuleEnd, const float& capsuleRadius) {
	// 未使用状態のオブジェクト取得
	GameObjectPtr characterObject = GameObjectManager::GetInstance().GetUnuseObject();
	// キャラクターコンポーネント追加
	characterObject->AddComponent<T>();
	// コライダーコンポーネント追加
	CapsuleColliderPtr collider = characterObject->AddComponent<CapsuleCollider>();
	collider->capsule = { capsuleStart, capsuleEnd, capsuleRadius };
	// モデルコンポーネント追加
	characterObject->AddComponent<ModelRenderer>();
	// 重力コンポーネント追加
	characterObject->AddComponent<GravityComponent>();
	// データのセット
	characterObject->SetObjectData(name, position, rotation);
	// キャラクターを返す
	return characterObject;
}

/*
 *	初期化処理
 */
void CharacterManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
	// はじめに一定数生成
	createCharacterList.reserve(CREATE_CHARACTER_COUNT);
}

/*
 *	プレイヤー生成
 */
GameObjectPtr CharacterManager::GeneratePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	// プレイヤーのベース作成
	GameObjectPtr player = CreateCharacter<PlayerComponent>(name, position, rotation, AABBMin, AABBMax);
	// ウデアクションコンポーネント追加
	player->AddComponent<ArmActionComponent>();
	// カメラのターゲットに追加
	CameraManager::GetInstance().SetTarget(player);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(player);
	// 生成キャラクターリストに追加
	createCharacterList.push_back(player);
	return player;
}

GameObjectPtr CharacterManager::GeneratePlayer(const std::string& name, const Vector3& position, const Vector3& rotation, const Vector3& capsuleStart, const Vector3& capsuleEnd, const float& capsuleRadius) {
	// プレイヤーのベース作成
	GameObjectPtr player = CreateCharacter<PlayerComponent>(name, position, rotation, capsuleStart, capsuleEnd, capsuleRadius);
	// ウデアクションコンポーネント追加
	player->AddComponent<ArmActionComponent>();
	// カメラのターゲットに追加
	CameraManager::GetInstance().SetTarget(player);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(player);
	// 生成キャラクターリストに追加
	createCharacterList.push_back(player);
	return player;
}

/*
 *	エネミー生成
 */
GameObjectPtr CharacterManager::GenerateEnemy(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	// 敵のベース作成
	GameObjectPtr enemy = CreateCharacter<EnemyComponent>(name, position, rotation, AABBMin, AABBMax);
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(enemy);
	// 生成キャラクターリストに追加
	createCharacterList.push_back(enemy);
	return enemy;
}

/*
 *	ID指定のキャラクター削除
 */
void CharacterManager::RemoveCharacter(int ID) {
	// 削除キャラクターの取得
	GameObjectPtr destroyObject = GameObjectUtility::GetUseObject(ID);
	// オブジェクトのリセット
	GameObjectUtility::ResetObject(destroyObject);
	// リスト内の削除オブジェクトを探す
	auto listBegin = createCharacterList.begin();
	auto listEnd = createCharacterList.end();
	auto destroyNumber = std::find(listBegin, listEnd, destroyObject);
	
	if (destroyNumber == listEnd) return;
	// リストから削除
	createCharacterList.erase(destroyNumber);
}
/*
 *	全てのキャラクター削除処理
 *  @author	Seki
 */
void CharacterManager::RemoveAllCharacter() {
	// リストの後ろから削除
	for (int i = createCharacterList.size() - 1; i >= 0; --i) {
		if (!createCharacterList[i]) continue;
		RemoveCharacter(createCharacterList[i]->ID);
	}
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
