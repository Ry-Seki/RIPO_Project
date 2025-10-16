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
	Vector3 currentMousePosition;	// マウスの現在の位置
	Vector3 previousMousePosition;	// マウスの直前の位置
	Vector3 mouseMoveValue;			// マウスの移動量
	float sensitivity;				// 感度

public:
	CameraComponent();
	virtual ~CameraComponent() = default;

public:
	virtual void Update(float deltaTime) override;
};
// 別名定義
using CameraComponentPtr = std::shared_ptr<CameraComponent>;

#endif // !_CAMERACOMPONENT_H_
