/*
 *	@file	Treasure.h
 *  @author oorui
 */

#ifndef _TREASURE_H_
#define _TREASURE_H_

#include "../StageObjectBase.h"


 /*
  *	お宝クラス
  */
class Treasure : public StageObjectBase {
private:
	bool isCollected;			// 取得済みかどうか
	int modelHandle;			// モデルのハンドル
	float viewRadius;			// 取得範囲

	int treasureID = -1;		// お宝のID
public:
	Treasure();
	virtual ~Treasure();

public:
	/*
	 *	@function	ModelLoad
	 *  @brief		モデルの読み込み
	 *  @param		const int modelHandleBase
	 */
	void ModelLoad(const int modelHandleBase)override;

	/*
	 *	@function	Update
	 *  @brief		更新
	 *  @param		float deltaTime
	 */
	void Update(float deltaTime)override;

	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

public:

	/*
	 *	取得済み判定
	 *  @return bool
	 */
	bool GetCollected() const { return isCollected; }

	/*
	 *	IDの取得
	 */
	int GetId() const { return treasureID; }

	/*
	 *	IDの変更
	 *  @param	int
	 */
	int SetID(int setValue) { treasureID = setValue; }

};


#endif // !_TREASURE_H_


