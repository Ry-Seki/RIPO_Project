/*
 *	@file	OBBCollider.h
 *	@author Riku
 */

#include "OBBCollider.h"
#include "../GameObject.h"

void OBBCollider::Update(float deltaTime) {
	isHit = false;
}

void OBBCollider::OnCollision(
	const std::shared_ptr<Component>& self, 
	const std::shared_ptr<Component>& other) {
	isHit = true;
}

void OBBCollider::DebugRender() {
	// hit判定によるカラー変更
	unsigned int color;
	if (isHit) {
		color = GetColor(0, 128, 0);
	}
	// 判定を取らない場合もカラー変更
	else if (!isCollider) {
		color = GetColor(128, 0, 0);
	}
	else {
		color = GetColor(255, 255, 255);
	}

	auto owner = GetOwner();
	// スケール適応
	Vector3 scaleSize = Scale(obb.size, owner->scale);
	// OBBのcenterを原点としたローカルボックス
	std::vector<Vector3> localBoxPoints = {
		-obb.size,
		{ -obb.size.x, -obb.size.y,  obb.size.z },
		{  obb.size.x, -obb.size.y, -obb.size.z },
		{  obb.size.x, -obb.size.y,  obb.size.z },
		{ -obb.size.x,  obb.size.y,  obb.size.z },
		obb.size,
		{  obb.size.x, obb.size.y, -obb.size.z },
		{ -obb.size.x, obb.size.y, -obb.size.z }
	};

	// ワールド座標のOBB
	std::vector<VECTOR> obbPoints;
	for (auto& point : localBoxPoints) {
		// ローカルボックスを回転させてワールド座標変換
		obbPoints.push_back(ToVECTOR(RotateY(point, obb.angle) + obb.center + owner->position));
	}

	// 描画
	DrawLine3D(obbPoints[0], obbPoints[1], color);
	DrawLine3D(obbPoints[0], obbPoints[2], color);
	DrawLine3D(obbPoints[3], obbPoints[1], color);
	DrawLine3D(obbPoints[3], obbPoints[2], color);
	DrawLine3D(obbPoints[0], obbPoints[7], color);
	DrawLine3D(obbPoints[1], obbPoints[4], color);
	DrawLine3D(obbPoints[2], obbPoints[6], color);
	DrawLine3D(obbPoints[3], obbPoints[5], color);
	DrawLine3D(obbPoints[7], obbPoints[4], color);
	DrawLine3D(obbPoints[7], obbPoints[6], color);
	DrawLine3D(obbPoints[5], obbPoints[4], color);
	DrawLine3D(obbPoints[5], obbPoints[6], color);
}
