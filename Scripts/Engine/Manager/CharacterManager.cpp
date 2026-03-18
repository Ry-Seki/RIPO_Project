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
#include "../Component/AnimatorComponent.h"
#include "../Component/Character/HPBarComponent.h"
#include "../Component/Character/BossHPBarComponentr.h"
#include "../Component/MoveComponent.h"
#include "../Component/HPComponent.h"
#include "../Component/StaminaComponent.h"
#include "../Component/StrengthComponent.h"
#include "../Component/ResistTimeComponent.h"

CharacterManager::CharacterManager()
	: engine(nullptr) {
}

/*
 *	キャラクター生成
 *	@param	setID			キャラクターの識別ID
 *	@param	name			キャラクターの名前
 *	@param	position		生成位置
 *  @param	rotation		生成角度
 *  @param	capsuleStart	カプセルの下端
 *  @param	capsuleEnd		カプセルの上端
 *  @param	capsuleRadius	カプセルの半径
 */
template <typename T>
GameObjectPtr CharacterManager::CreateCharacter(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& capsuleStart,
	const Vector3& capsuleEnd,
	const float& capsuleRadius) {
	// 未使用状態のオブジェクト取得
	GameObjectPtr characterObject = GameObjectManager::GetInstance().GetUnuseObject();
	// キャラクターコンポーネント追加
	characterObject->AddComponent<T>();
	// カプセルコライダー追加
	CapsuleColliderPtr capsuleCollider = characterObject->AddComponent<CapsuleCollider>();
	capsuleCollider->capsule = { capsuleStart, capsuleEnd, capsuleRadius };

	// モデルコンポーネント追加
	characterObject->AddComponent<ModelRenderer>();
	// 重力コンポーネント追加
	characterObject->AddComponent<GravityComponent>();
	// アニメーターコンポーネント追加
	characterObject->AddComponent<AnimatorComponent>();
	// 移動コンポーネント追加
	characterObject->AddComponent<MoveComponent>();
	// HPコンポーネント追加
	characterObject->AddComponent<HPComponent>();
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
 *	@param	name			プレイヤーの名前
 *	@param	position		生成位置
 *  @param	rotation		生成角度
 *  @param	capsuleStart	カプセルの下端
 *  @param	capsuleEnd		カプセルの上端
 *  @param	capsuleRadius	カプセルの半径
 */
GameObjectPtr CharacterManager::GeneratePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& capsuleStart,
	const Vector3& capsuleEnd,
	const float& capsuleRadius) {
	// プレイヤーのベース作成
	player = CreateCharacter<PlayerComponent>(name, position, rotation, capsuleStart, capsuleEnd, capsuleRadius);
	// ウデアクションコンポーネント追加
	player->AddComponent<ArmActionComponent>();
	// スタミナコンポーネント追加
	player->AddComponent<StaminaComponent>();
	// ストレングスコンポーネント追加
	player->AddComponent<StrengthComponent>();
	// レジストタイムコンポーネント追加
	player->AddComponent<ResistTimeComponent>();
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
 *	@param	name			エネミーの名前
 *	@param	position		生成位置
 *  @param	rotation		生成角度
 *  @param	AABBMin			AABBの各軸に置ける最小値
 *  @param	AABBMax			AABBの各軸に置ける最大値
 *  @param	capsuleStart	カプセルの下端
 *  @param	capsuleEnd		カプセルの上端
 *  @param	capsuleRadius	カプセルの半径
 *  @author	kuu
 */
GameObjectPtr CharacterManager::GenerateEnemy(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax,
	const Vector3& capsuleStart,
	const Vector3& capsuleEnd,
	const float& capsuleRadius,
	int enemyID) {
	// 敵のベース作成
   	enemy = CreateCharacter<EnemyComponent>(name, position, rotation, capsuleStart, capsuleEnd, capsuleRadius);
	// HPバーコンポーネント追加
	enemy->AddComponent<HPBarComponent>();
	// AABBコライダーコンポーネント追加
	AABBColliderPtr aabbCollider = enemy->AddComponent<AABBCollider>();
	aabbCollider->aabb = { AABBMin, AABBMax };
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(enemy);
	auto component = enemy->GetComponent<EnemyComponent>();
	if (component) {
		component->SetEnemyStart(enemyID);
	}
	// 生成キャラクターリストに追加
	createCharacterList.push_back(enemy);
	return enemy;
}

/*
 *	ボス生成
 *	@param	name			ボスの名前
 *	@param	position		生成位置
 *  @param	rotation		生成角度
 *  @param	AABBMin			AABBの各軸に置ける最小値
 *  @param	AABBMax			AABBの各軸に置ける最大値
 *  @param	capsuleStart	カプセルの下端
 *  @param	capsuleEnd		カプセルの上端
 *  @param	capsuleRadius	カプセルの半径
 *  @author	kuu
 */
GameObjectPtr CharacterManager::GenerateBoss(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax,
	const Vector3& capsuleStart,
	const Vector3& capsuleEnd,
	const float& capsuleRadius,
	const int bossID) {
	// ボスのベース作成
	boss = CreateCharacter<BossComponent>(name, position, rotation, capsuleStart, capsuleEnd, capsuleRadius);
	// ボスのHPバーコンポーネント追加
	boss->AddComponent<BossHPBarComponent>();
	// AABBコライダーコンポーネント追加
	AABBColliderPtr aabbCollider = boss->AddComponent<AABBCollider>();
	aabbCollider->aabb = { AABBMin, AABBMax };
	// シーンが持つゲームオブジェクト配列に追加
	engine->AddGameObject(boss);
	// 生成キャラクターリストに追加
	createCharacterList.push_back(boss);
	auto component = boss->GetComponent<BossComponent>();
	if (component) {
		// IDによって挙動を変える
		component->SetBossStart(bossID + 100);
	}
	return boss;
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
	auto animator = gameObject->GetComponent<AnimatorComponent>();
	if (!animator) return;

	modelRenderer->SetModelHandle(modelHandle);
	animator->SetModelHandle(modelRenderer->GetModelHandle());
	animator->LoadIndex(true);
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
