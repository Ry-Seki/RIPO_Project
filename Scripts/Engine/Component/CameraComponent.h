/*
 *	@file	CameraComponent.h
 *  @autor	Riku
 */

#ifndef _CAMERACOMPONENT_H_
#define _CAMERACOMPONENT_H_

#include "Component.h"
#include "../Engine.h"
#include "../VecMath.h"

class CameraComponent : public Component {
private:
	Vector3 mousePosition;			// マウスの位置
	Vector3 mouseMoveValue;			// マウスの移動量
	float sensitivity;				// 感度
	float playerDistancePos;		// プレイヤーから離れる距離

	const float CAMERA_ROTATION_X_MAX;		// カメラの角度のxの最大
	const float CAMERA_ROTATION_X_MIN;		// カメラの角度のxの最小
	const float PLAYER_HEAD_HEIGHT;			// プレイヤーの頭の高さ

public:
	CameraComponent();
	~CameraComponent() = default;

public:
	void Update(float deltaTime) override;
};
// 別名定義
using CameraComponentPtr = std::shared_ptr<CameraComponent>;

#endif // !_CAMERACOMPONENT_H_
