/*
 *	@file	StageBase.h
 *  @author kuu & oorui
 */


#ifndef _STAGEBASE_H_
#define _STAGEBASE_H_
#include <DxLib.h>
#include <string>
#include "../Load/Model/LoadModel.h"
#include "../VecMath.h"

class GameObject;

class StageBase {
protected:
	// モデルハンドル
	int modelHandle;
	GameObject* player;
	Vector3 playerMove;
public:
	// コンストラクタ
	StageBase() : modelHandle(-1) {}
	virtual ~StageBase() = default;

protected:
	/*
	 *	片付け処理
	 */
	void Clean(int MHandle) {
		if (MHandle >= 0) {
			MV1SetVisible(MHandle, false);
		}
	}

public:


	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Render() = 0;

	// ステージの当たり判定の描画
	virtual void StageColliderRenderer(GameObject* other,Vector3 MoveVec) = 0;

	// 終了処理
	virtual void Execute() = 0;


	/*
	 *	当たり判定の更新
	 *  @param Vector3* position
	 *  @param Vector3  MoveVec
	 */
	virtual void UpdateCollision(GameObject* other, Vector3 MoveVec) = 0;

	/*
	 *	ステージハンドルセット
	 */
	virtual void SetModelHandle(const int modelHandleBase) = 0;

	/*
	 *	ステージモデルの取得
	 *  @return modelHandle
	 */
	virtual int GetStageModelHandle()const { return modelHandle; }

	virtual void SetGameObject(GameObject* other) { player = other; }
	virtual void SetGameObjectVec(Vector3 vec) { playerMove = vec; }

};

#endif // !_STAGEBASE_H_