/*
 *	@file	Treasure.h
 *  @author oorui
 */

#ifndef _TREASURE_H_
#define _TREASURE_H_

#include "../StageObjectBase.h"

 // お宝の種類
enum class TreasureType {
	TreaureA,	// お宝A
	TreaureB,	// お宝B
	TreaureC,	// お宝C
	TreaureD,	// お宝D
};

/*
 *	お宝クラス
 */
class Treasure : public StageObjectBase {
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



};


#endif // !_TREASURE_H_


