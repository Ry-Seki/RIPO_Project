/*
 *	@file	StageObjectManager.h
 *	@author kuu
 */

#ifndef _STAGEOBJECTMANAGER_H_
#define _STAGEOBJECTMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include "../Stage/StageObject/StageObjectBase.h"
#include "../Stage/StageObject/ExitPoint.h"

/*
 *	ステージオブジェクトの管理クラス
 */
class StageObjectManager : public Singleton<StageObjectManager> {
	// フレンド宣言
	friend class Singleton<StageObjectManager>;

private:
	Engine* engine;
	ExitPoint* exitPoint;
	StageObjectBaseList createStageObjectList;
	GameObjectPtr stageObjectObj;

	const size_t CREATE_STAGEOBJ_COUNT = 16;	// 事前に生成する数

private:
	StageObjectManager();
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
	 */
	template <typename T>
	StageObjectBasePtr CreateStageObject(
		const int setID,
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

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
	 */
	void GenerateTreasure(
		const std::string& name,
		const Vector3& position,
		const Vector3& rotation,
		const Vector3& AABBMin,
		const Vector3& AABBMax);

	/*
	 *	ID指定のステージオブジェクト削除
	 */
	void RemoveStageObject(int stageObjectID);

	/* 
	 *	ID指定のステージオブジェクト取得
	 */
	StageObjectBasePtr GetStageObject(int stageObjectID);

	/*
	 *	脱出フラグの取得
	 */
	inline bool GetExitFlag() const {
		return exitPoint->GetExitTriger();
	}


};

#endif // !_STAGEOBJECTMANAGER_H_
