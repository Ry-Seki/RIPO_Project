/*
 *	@file	@Treasure.cpp
 *  @author	oorui
 */

#include "Treasure.h"
#include "../Scripts/Engine/GameConst.h"
#include "../Scripts/Engine/VecMath.h"
 /*
  *	コンストラクタ
  */
Treasure::Treasure()
	: StageObjectBase()
	, isCollected(false)
	, modelHandle(-1)
	, viewRadius(GameConst::VIEW_RADIUS) {
}

Treasure::~Treasure() {

}

/*
  *	@function	ModelLoad
  *  @brief		モデルの読み込み
  *  @param		const int modelHandleBase
  */
void Treasure::ModelLoad(const int modelHandleBase) {
	// 種類別でモデルを読み込む
	modelHandle = modelHandleBase;

}


/*
 *	@function	Update
 *  @brief		更新
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {
	if (isCollected) return;


	// 当たり判定の更新

}


/*
 *	衝突が起きたときに呼び出される処理
 */
void Treasure::OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) {
}


/*
 *	@function	IsInView
 *  @brief		プレイヤーが範囲内か判定
 *  @param		const Vector& playerPos
 */
bool Treasure::IsInView(const Vector3& playerPos) const {
	// お宝自身の座標を取得
	Vector3 treasurePos = GetPosition();

	// 二点間の距離を求める
	float distance = Vector3::Distance(treasurePos, playerPos);

	// 取得範囲内であれば
	return (distance <= viewRadius);
}

/*
 *	取得されたとき
 */
void Treasure::Collect() {
	// すでに取得していたら
	if (isCollected)return;


	isCollected = true;
	isHeld = true;

}

