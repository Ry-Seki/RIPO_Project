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
	Vector3 ofssetFromePlayer;	// プレイヤーからの相対座標
	bool isHeld;				// プレイヤーの持たれているかどうか

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

	/*
	 *	@function	IsInView
	 *  @brief		プレイヤーが範囲内か判定
	 *  @param		const Vector3& playerPos
	 */
	bool IsInView(const Vector3& playerPos) const;

	/*
	 *	取得されたとき
	 */
	void Collect();



public:

	/*
	 *	取得済み判定
	 */
	bool GetCollected() const { return isCollected; }

	/*
	 *	持たれたかどうかの設定
	 */
	void SetHeld(bool setValue) {};

	/*
	 *	持たれているかどうかの取得
	 */
	bool GetHeld()const { return isHeld; }

};


#endif // !_TREASURE_H_


