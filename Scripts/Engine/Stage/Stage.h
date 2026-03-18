/*
 *  @file Stage.h
 *  @author oorui
 */

#ifndef _STAGE_H_
#define _STAGE_H_

#include "StageBase.h"
#include <string>
#include <vector>
#include "../VecMath.h"
#include "../GameObject.h"

 // 前方宣言
// class GameObject;
class StageCollision;



/*
 *  ステージクラス
 */
class Stage : public StageBase {
private:
	std::string modelPath;						// モデルのパス
	
	std::unique_ptr<StageCollision> collision;	// 当たり判定管理クラス
	GameObjectPtr player;				// プレイヤー


public:
	Stage();
	virtual ~Stage() override;

	/*
	 *	ステージデータの読み込み
	 *  @pram const int modelHandle
	 */
	void SetModelHandle(const int modelHandleBase)override;

	/*
	 *	更新
	 */
	void Update() override;

	/*
	 *	描画
	 */
	void Render() override;

	/*
	 *	終了処理
	 */
	void Execute() override;

	/*
	 * @brief ステージの当たり判定を更新
	 * @param position     オブジェクト位置
	 * @param MoveVec      移動ベクトル
	 */
	void UpdateCollision(GameObject* other, Vector3 MoveVec) override;

public:
	/*
	 *	モデルハンドルの取得
	 */
	int GetModelHandle() const { return modelHandle; }


};


#endif // !_STAGE_H_