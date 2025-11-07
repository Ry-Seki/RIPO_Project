/*
 *	@file	StageObjectManager.cpp
 *	@author kuu & oorui
 */
#include "StageObjectManager.h"
#include "GameObjectManager.h"
#include "../Stage/StageObject/Treasure/Treasure.h"
#include "../Stage/StageObject/StageObjectBase.h"
#include "../Component/ModelRenderer.h"

StageObjectManager::StageObjectManager() {
}

/*
 *	ステージオブジェクト生成
 */
template <typename T>
StageObjectBasePtr StageObjectManager::CreateStageObject(
	int setID,
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax,
	GameObjectPtr& stageObject
) {
	// 未使用のオブジェクト取得
	stageObject = GameObjectManager::GetInstance().GetUnuseObject();
	// ステージオブジェクト生成
	StageObjectBasePtr createStageObj = stageObject->AddComponent<T>();
	// コライダー生成
	AABBColliderPtr collider = stageObject->AddComponent<AABBCollider>();
	collider->aabb = { AABBMin, AABBMax };
	// ID設定
	createStageObj->SetID(setID);
	// データのセット
	stageObject->SetObjectData(name, position, rotation);
	// 出口オブジェクトを返す
	return createStageObj;
}

/*
 *	初期化
 */
void StageObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
	// 最初に一定数生成
	createStageList.reserve(CREATE_STAGEOBJ_COUNT);
	for (size_t i = 0; i < CREATE_STAGEOBJ_COUNT; i++) {
		// 空の要素を生成
		createStageList.push_back(nullptr);
		createStageObjectList.push_back(nullptr);
	}
}

/*
 *	出口生成
 */
void StageObjectManager::GenerateExit(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	GameObjectPtr createExitPoint;
	// リストの要素の数
	size_t stageObjectListCount = createStageList.size();
	// 生成ステージオブジェクトの空きをチェック
	for (size_t i = 0; i < stageObjectListCount; i++) {
		if (createStageList[i] != nullptr) continue;
		// リストの空きに生成
		createStageList[i] = CreateStageObject<ExitPoint>(i, name, position, rotation, AABBMin, AABBMax, createExitPoint);
		exitPoint = createStageList[i]->GetOwner()->GetComponent<ExitPoint>().get();
		// オブジェクトのリストに保存
		createStageObjectList[i] = createExitPoint;

		// シーンが持つゲームオブジェクト配列に入れる
		engine->AddGameObject(createExitPoint);
		return;
	}
	// 空きがなかったら一番後ろに生成
	StageObjectBasePtr stageObject = CreateStageObject<ExitPoint>(0, name, position, rotation, AABBMin, AABBMax, createExitPoint);
	exitPoint = stageObject->GetOwner()->GetComponent<ExitPoint>().get();
	createStageList.push_back(stageObject);
	// オブジェクトリストに保存
	createStageObjectList.push_back(createExitPoint);
	engine->AddGameObject(createExitPoint);
}

/*
 *	お宝生成
 *  @param	name		お宝の名前
 *  @param	position	生成位置
 *  @param	rotation	生成角度
 *  @param	AABBMin		AABBの各軸における最小値
 *  @param	AABBMax		AABBの各軸における最大値
 */
void StageObjectManager::GenerateTreasure(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	GameObjectPtr treasure;
	// リストの要素の数
	size_t stageObjectListCount = createStageList.size();
	// 生成ステージオブジェクトの空きをチェック
	for (size_t i = 0; i < stageObjectListCount; i++) {
		if (createStageList[i] != nullptr) continue;
		// リストの空きに生成
		createStageList[i] = CreateStageObject<Treasure>(i, name, position, rotation, AABBMin, AABBMax, treasure);
		createStageList[i]->GetOwner()->AddComponent<ModelRenderer>();

		// オブジェクトのリストに保存
		//createStageObjectList[i] = treasure;

		// シーンが持つゲームオブジェクト配列に入れる
		engine->AddGameObject(treasure);

		return;
	}
	// 空きがなかったら一番後ろに生成
	StageObjectBasePtr stageObject = CreateStageObject<Treasure>(0, name, position, rotation, AABBMin, AABBMax, treasure);
	stageObject->GetOwner()->AddComponent<ModelRenderer>();
	createStageList.push_back(stageObject);
	// オブジェクトリストに保存
	//createStageObjectList.push_back(treasure);
	engine->AddGameObject(treasure);
}

/*
 *	階段生成
 *  @param	name		階段の名前
 *  @param	position	生成位置
 *  @param	rotation	生成角度
 *  @param	AABBMin		AABBの各軸における最小値
 *  @param	AABBMax		AABBの各軸における最大値
 */
void StageObjectManager::GenerateStair(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax) {
	GameObjectPtr createStair;
	// リストの要素の数
	size_t stageObjectListCount = createStageList.size();
	// 生成ステージオブジェクトの空きをチェック
	for (size_t i = 0; i < stageObjectListCount; i++) {
		if (createStageList[i] != nullptr) continue;
		// リストの空きに生成
		createStageList[i] = CreateStageObject<Stair>(i, name, position, rotation, AABBMin, AABBMax, createStair);
		stair = createStageList[i]->GetOwner()->GetComponent<Stair>().get();
		// オブジェクトのリストに保存
		createStageObjectList[i] = createStair;

		// シーンが持つゲームオブジェクト配列に入れる
		engine->AddGameObject(createStair);
		return;
	}
	// 空きがなかったら一番後ろに生成
	StageObjectBasePtr stageObject = CreateStageObject<Stair>(0, name, position, rotation, AABBMin, AABBMax, createStair);
	stair = stageObject->GetOwner()->GetComponent<Stair>().get();
	createStageList.push_back(stageObject);
	// オブジェクトリストに保存
	createStageObjectList.push_back(createStair);
	engine->AddGameObject(createStair);
}

/*
 *	ID指定のステージオブジェクト削除
 *  @param[in]	int stageObject			削除するオブジェクトのID
 */
void StageObjectManager::RemoveStageObject(int stageObjectID) {

	GameObjectPtr destroyObject = createStageObjectList[stageObjectID];
	// オブジェクトのリセット
	GameObjectManager::GetInstance().ResetObject(destroyObject);
	// リストから削除
	createStageList[stageObjectID] = nullptr;
	// オブジェクトリストからも削除
	createStageObjectList[stageObjectID] = nullptr;
}
/*
 *	全てのステージオブジェクト削除
 *  @author		Seki
 */
void StageObjectManager::RemoveAllStageObject() {
	for (int i = 0, max = createStageList.size(); i < max; i++) {
		RemoveStageObject(i);
	}
}

/*
 *	ID指定のステージオブジェクト取得
 */
StageObjectBasePtr StageObjectManager::GetStageObject(int stageObjectID) {
	return createStageList[stageObjectID];
}
/*
 *	お宝にモデルハンドルをセット
 *	@param[in]	GameObject* gameObject		セットするモデル
 *  @param[in]	const int modelHandle		モデルハンドル
 *  @author		Seki
 */
void StageObjectManager::SetModelHandle(GameObject* gameObject, const int modelHandle) {
	if (!gameObject) return;

	auto modelRenderer = gameObject->GetComponent<ModelRenderer>();
	if (!modelRenderer) return;

	modelRenderer->SetModelHandle(modelHandle);
}
/*
 *	ステージオブジェクトのオーナーオブジェクトの取得
 *  @param[in]	const StageObjectBasePtr& setStageObject
 *  @return		GameObject*
 *  @author		Seki
 */
GameObject* StageObjectManager::GetStageObjectOwner(const StageObjectBasePtr& setStageObject) const {
	if (!setStageObject) return nullptr;
	GameObject* owner = setStageObject->GetOwner();
	if (!owner) return nullptr;

	return owner;
}
