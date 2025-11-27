/*
 *	@file	StageObjectUtility.h
 *	@author	Seki
 */

#ifndef _STAGE_OBJECT_UTILITY_H_
#define _STAGE_OBJECT_UTILITY_H_

#include "../../Manager/StageObjectManager.h"

 /*
  *	ステージオブジェクト関連実行処理
  *  @note	名前空間での省略可能	using namespace StageObjectUtility
  */
namespace StageObjectUtility {
	/*
	 *	出口生成
	 *  @param	name		出口の名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 */
	inline void GenerateExit(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax) {
		StageObjectManager::GetInstance().GenerateExit(name, position, rotation, AABBMin, AABBMax);
	}
	/*
	 *	お宝生成
	 *  @param	name		お宝の名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 */
	inline void GenerateTreasure(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax,
		const int setTreasureID
	) {
		StageObjectManager::GetInstance().GenerateTreasure(name, position, rotation, AABBMin, AABBMax, setTreasureID);
	}
	/*
	 *	階段生成
	 *  @param	name		階段の名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 */
	inline void GenerateStair(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax
	) {
		StageObjectManager::GetInstance().GenerateStair(name, position, rotation, AABBMin, AABBMax);
	}
	/*
	 *	ID指定のステージオブジェクト削除
	 *  @param[in]	int stageObjectID		削除するオブジェクトのID
	 */
	inline void RemoveStageObject(int stageObjectID) {
		StageObjectManager::GetInstance().RemoveStageObject(stageObjectID);
	}
	/*
	 *	ポインタ指定のステージオブジェクト削除
	 *	@param[in]	const StageObjectBasePtr& destroyObject		削除するオブジェクトのID
	 */
	inline void RemoveStageObject(const StageObjectBasePtr& destroyObject) {
		if (!destroyObject) return;
		int stageObjectID = destroyObject->GetID();
		StageObjectManager::GetInstance().RemoveStageObject(stageObjectID);
	}
	/*
	 *	全てのステージオブジェクト削除
	 */
	inline void RemoveAllStageObject() {
		StageObjectManager::GetInstance().RemoveAllStageObject();
	}
	/*
	 *	@brief		ステージオブジェクト配列の取得
	 *	@return		GameObjectList&
	 */
	inline GameObjectList& GetCreateObjectList() {
		return StageObjectManager::GetInstance().GetCreateObjectList();
	}
	/*
	 *	@brief		ID指定のステージオブジェクト取得
	 *  @return		StageObjectBasePtr
	 */
	inline StageObjectBasePtr GetStageObject(int stageObjectID) {
		return StageObjectManager::GetInstance().GetStageObject(stageObjectID);
	}
	/*
	 *	@brief		名前指定のステージオブジェクトの取得
	 *  @param[in]	const std::string& objectName
	 *	@return		GameObjectPtr
	 */
	inline GameObjectPtr GetStageObject(const std::string& objectName) {
		return StageObjectManager::GetInstance().GetStageObject(objectName);
	}
	/*
	 *	脱出フラグの取得
	 *  @return		bool
	 */
	inline bool GetExitFlag() {
		return StageObjectManager::GetInstance().GetExitFlag();
	}
	/*
	 *	階段オブジェクトに触れていたかどうか
	 *  @return		bool
	 */
	inline bool GetStairMove() {
		return StageObjectManager::GetInstance().GetStairMove();
	}
	/*
	 *	お宝にモデルハンドルをセット
	 *	@param[in]	GameObject* gameObject		セットするモデル
	 *  @param[in]	const int modelHandle		モデルハンドル
	 */
	inline void SetModelHandle(GameObject* gameObject, const int modelHandle) {
		StageObjectManager::GetInstance().SetModelHandle(gameObject, modelHandle);
	}
	/*
	 *	ステージオブジェクトのオーナーオブジェクトの取得
	 *  @param[in]	const StageObjectBasePtr& setStageObject
	 *  @return		GameObject*
	 */
	inline GameObject* GetStageObjectOwner(const StageObjectBasePtr& setStageObject) {
		return StageObjectManager::GetInstance().GetStageObjectOwner(setStageObject);
	}
};

#endif // !_STAGE_OBJECT_UTILITY_H_
