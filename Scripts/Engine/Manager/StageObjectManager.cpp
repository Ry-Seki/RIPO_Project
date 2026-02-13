/*
 *	@file	StageObjectManager.cpp
 *	@author kuu & oorui & Seki
 */
#include "StageObjectManager.h"
#include "../GameObject/GameObjectUtility.h"
#include "../Stage/StageObject/Treasure/Treasure.h"
#include "../Stage/StageObject/StageObjectBase.h"
#include "../Component/ModelRenderer.h"
#include "../../Data/TreasureDataList.h"

using namespace GameObjectUtility;


/*
 *	初期化
 */
void StageObjectManager::Initialize(Engine& setEngine) {
	engine = &setEngine;
	// 最初に一定数生成
	createStageObjectList.reserve(CREATE_STAGEOBJ_COUNT);
}
/*
 *	ステージオブジェクト生成
 */
template <typename T>
StageObjectBasePtr StageObjectManager::CreateStageObject(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation,
	const Vector3& AABBMin,
	const Vector3& AABBMax,
	GameObjectPtr& stageObject,
	int setTreasreID) {
	// 未使用のオブジェクト取得
	stageObject = GetUnuseObject();
	// ステージオブジェクト生成
	StageObjectBasePtr createStageObj = stageObject->AddComponent<T>();
	// コライダー生成
	AABBColliderPtr collider = stageObject->AddComponent<AABBCollider>();
	collider->aabb = { AABBMin, AABBMax };
	// ID設定
	createStageObj->SetTreasureID(setTreasreID);
	// データのセット
	stageObject->SetObjectData(name, position, rotation);
	// 出口オブジェクトを返す
	return createStageObj;
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
	// 生成ステージオブジェクトの空きをチェック
	// リストの空きに生成
	CreateStageObject<ExitPoint>(name, position, rotation, AABBMin, AABBMax, createExitPoint);
	exitPoint = createExitPoint->GetComponent<ExitPoint>().get();
	// オブジェクトのリストに保存
	createStageObjectList.push_back(createExitPoint);
	// シーンが持つゲームオブジェクト配列に入れる
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
	const Vector3& AABBMax,
	int setTreasureID) {
	GameObjectPtr treasure;
	// 生成ステージオブジェクトの空きをチェック
	// リストの空きに生成
	CreateStageObject<Treasure>(name, position, rotation, AABBMin, AABBMax, treasure, setTreasureID);
	treasure->AddComponent<ModelRenderer>();

	auto component = treasure->GetComponent<Treasure>();
	if (component) {
		TreasureStatusData data{};
		TreasureValueDataList::TryGetTreasureStatus(setTreasureID, data);
		component->SetEffectName(data.effectName);
		component->SetStrength(data.strength);
	}

	// オブジェクトのリストに保存
	createStageObjectList.push_back(treasure);
	// シーンが持つゲームオブジェクト配列に入れる
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
	// リストの空きに生成
	CreateStageObject<Stair>(name, position, rotation, AABBMin, AABBMax, createStair);
	stairList.push_back(createStair->GetComponent<Stair>().get());
	// オブジェクトのリストに保存
	createStageObjectList.push_back(createStair);
	// シーンが持つゲームオブジェクト配列に入れる
	engine->AddGameObject(createStair);
}

/*
 *	GameObjectのID指定のステージオブジェクト削除
 *  @param[in]	int ID					削除するオブジェクトのID
 */
void StageObjectManager::RemoveStageObject(int ID) {
	GameObjectPtr destroyObject = GameObjectUtility::GetUseObject(ID);
	// オブジェクトのリセット
	ResetObject(destroyObject);
	// リストから削除
	auto begin = createStageObjectList.begin();
	auto end = createStageObjectList.end();
	auto itr = std::find(createStageObjectList.begin(), createStageObjectList.end(), destroyObject);

	if (itr == end) return;
	// リストから削除
	createStageObjectList.erase(itr);
}
/*
 *	@brief		全てのステージオブジェクト削除
 *  @author		Seki
 */
void StageObjectManager::RemoveAllStageObject() {
	for (int i = createStageObjectList.size() - 1; i >= 0; i--) {
		if (!createStageObjectList[i]) continue;
		RemoveStageObject(createStageObjectList[i]->ID);
	}
}
/*
 *	@brief	ID指定のオブジェクト取得
 */
StageObjectBasePtr StageObjectManager::GetStageObject(int stageObjectID) {
	auto stageObject = GetUseObject(stageObjectID)->GetComponent<StageObjectBase>();
	if (!stageObject) return nullptr;

	return stageObject;
}
/*
 *	@brief		名前指定のステージオブジェクト取得
 *  @param[in]	const std::string& objectName
 *  @return		GameObjectPtr
 *	@author		Seki
 */
GameObjectPtr StageObjectManager::GetStageObject(const std::string& objectName) {
	for (int i = 0, max = createStageObjectList.size(); i < max; i++) {
		GameObjectPtr object = createStageObjectList[i];
		if (!object) continue;

		if (object->name != objectName) continue;

		return object;
	}
	return nullptr;
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
