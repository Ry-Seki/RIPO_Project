/*
 *	@file	StageObjectManager.h
 *	@author kuu & oorui
 */

#ifndef _STAGEOBJECTMANAGER_H_
#define _STAGEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include "../Stage/StageObject/StageObjectBase.h"
#include "../Stage/StageObject/ExitPoint.h"
#include "../Stage/StageObject/Stair.h"

#include <vector>

 /*
  *	ステージオブジェクトの管理クラス
  */
class StageObjectManager : public Singleton<StageObjectManager> {
	// フレンド宣言
	friend class Singleton<StageObjectManager>;

private:
	int moveStairID = -1;
	Engine* engine = nullptr;
	ExitPoint* exitPoint = nullptr;
	std::vector<Stair*> stairList;

	GameObjectList createStageObjectList;

	const size_t CREATE_STAGEOBJ_COUNT = 16;	// 事前に生成する数

private:
	StageObjectManager() = default;
	~StageObjectManager() = default;

private:
	/*
	 *	ステージオブジェクト生成
	 *  @param	setID		ステージオブジェクトの識別ID
	 *  @param	name		ステージオブジェクトの名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 *  @param  stageObject
	 */
	template <typename T>
	StageObjectBasePtr CreateStageObject(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax,
		GameObjectPtr& stageObject,
		int setTreasureID = -1
	);

public:
	/*
	 *	初期化
	 */
	void Initialize(Engine& setEngine);

	/*
	 *	出口生成
	 *  @param	name		出口の名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 */
	void GenerateExit(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

	/*
	 *	お宝生成
	 *  @param	name		お宝の名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 *  @param	setID		お方らの識別ID
	 */
	void GenerateTreasure(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax,
		const int setTreasureID
	);

	/*
	 *	階段生成
	 *  @param	name		階段の名前
	 *  @param	position	生成位置
	 *  @param	rotation	生成角度
	 *  @param	AABBMin		AABBの各軸における最小値
	 *  @param	AABBMax		AABBの各軸における最大値
	 *  @param	ID			階段固有のID
	 */
	void GenerateStair(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax
	);
	/*
	 *	ID指定のステージオブジェクト削除
	 *  @param[in]	int stageObjectID		削除するオブジェクトのID
	 */
	void RemoveStageObject(int stageObjectID);
	/*
	 *	全てのステージオブジェクト削除
	 *  @author		Seki
	 */
	void RemoveAllStageObject();

public:
	/*
	 *	ID指定のステージオブジェクト取得
	 */
	StageObjectBasePtr GetStageObject(int stageObjectID);
	/*
	 *	@brief		名前指定のステージオブジェクト取得
	 *  @param[in]	const std::string& objectName
	 *  @return		GameObjectPtr
	 *	@author		Seki
	 */
	GameObjectPtr GetStageObject(const std::string& objectName);
	/*
	 *	@brief		ステージオブジェクト配列の取得
	 *	@return		GameObjectList&
	 *  @author		Seki
	 */
	inline GameObjectList& GetCreateObjectList() { return createStageObjectList; }
	/*
	 *	@brief		移動先フロアIDの取得
	 *	@return		int
	 *	@author		Seki
	 */
	inline int GetMoveStairID() const { return moveStairID; }
	/*
	 *	@brief		移動先フロアIDの変更
	 *	@param[in]	int setID
	 *	@author		Seki
	 */
	inline void SetMoveStairID(int setID) {
		moveStairID = setID;
	}
	/*
	 *	脱出フラグの取得
	 *  @retrn bool
	 */
	inline bool GetExitFlag() const {
		if (!exitPoint)return false;
		return exitPoint->GetExitTriger();
	}

	/*
	 *	階段オブジェクトに触れていたかどうか
	 *  @return boool
	 *  @author oorui
	 */
	inline bool GetStairMove() const {
		for (int i = 0, max = stairList.size(); i < max; i++) {
			if (stairList[i]->GetStairMove()) return true;
		}
		return false;
	}

	/*
	 *	当たり判定フラグをすべて初期化する
	 */
	void ResetFlag() {
		for (int i = 0, max = stairList.size(); i < max; i++) {
			stairList[i]->SetStairMove(false);
		}
		if (exitPoint) exitPoint->SetExitTrigger(false);
	}
	void ClearObject() {
		stairList.clear();
		exitPoint = nullptr;
	}

public:
	/*
	 *	お宝にモデルハンドルをセット
	 *	@param[in]	GameObject* gameObject		セットするモデル
	 *  @param[in]	const int modelHandle		モデルハンドル
	 *  @author		Seki
	 */
	void SetModelHandle(GameObject* gameObject, const int modelHandle);
	/*
	 *	ステージオブジェクトのオーナーオブジェクトの取得
	 *  @param[in]	const StageObjectBasePtr& setStageObject
	 *  @return		GameObject*
	 *  @author		Seki
	 */
	GameObject* GetStageObjectOwner(const StageObjectBasePtr& setStageObject) const;
};

#endif // !_STAGEOBJECTMANAGER_H_
