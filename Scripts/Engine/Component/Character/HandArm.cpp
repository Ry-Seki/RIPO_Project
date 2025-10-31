/*
 *	@file	HandArm.cpp
 *	@author Riku
 */

#include "HandArm.h"
#include "../../RayCast.h"

HandArm::HandArm()
	: liftObject(nullptr)
	, player(nullptr)
	, engine(nullptr)
	
	, LEFTABLE_DISTANCE(100)
{}

void HandArm::Update(float deltaTime) {
	player = GetOwner();
	engine = player->GetEngine();

	// 左クリックでお宝持ち上げ
	if (GetMouseInput() & MOUSE_INPUT_LEFT)
		LiftTreasure();


}

/*
 *	お宝持ち上げ処理
 */
void HandArm::LiftTreasure() {
	// 正面にオブジェクトがあるか
	float hitLength = 0;
	GameObject* hitObject;
	if (RayCast(engine, player->position, player->rotation, hitLength, hitObject)) {
		// 持ち上げ可能距離なら持ち上げ対象を保存
		if (hitLength < LEFTABLE_DISTANCE) return;
		liftObject = hitObject;
	}
}

/*
 *	お宝運び処理
 */
void HandArm::CarryTreasur() {
	if (liftObject == nullptr) return;

	liftObject->position = player->position;
}
