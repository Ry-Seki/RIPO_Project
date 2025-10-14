#include "CameraComponent.h"
#include "DxLib.h"

CameraComponent::CameraComponent()
	: currentPosition(Vector3::zero)
	, previousPosition(Vector3::zero) {
}

void CameraComponent::Update(float deltaTime) {
	// 直前の位置を更新
	previousPosition = currentPosition;
	// マウスの位置を取得
	int mouseX = 0, mouseY = 0;
	GetMousePoint(&mouseX, &mouseY);
	currentPosition.x = mouseX;
	currentPosition.y = mouseY;
	
	if (currentPosition == Vector3::zero ||
		previousPosition == Vector3::zero) return;

}
