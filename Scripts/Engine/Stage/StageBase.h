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
class StageBase {
protected:
	// モデルハンドル
	int modelHandle;

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
	// ステージ読み込み
	virtual void Load(const std::shared_ptr<LoadModel>& model) = 0;

	virtual void ModelLoad(const int modelHandleBase) = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Render() = 0;

	// 終了処理
	virtual void Execute() = 0;


	/*
	 *	当たり判定の更新
	 *  @param Vector3* position
	 *  @param Vector3  MoveVec
	 */
	virtual void UpdateCollision(Vector3* position, Vector3 MoveVec) = 0;
};

#endif // !_STAGEBASE_H_