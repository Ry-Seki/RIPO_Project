/*
 *	@file	CameraComponent.h
 *  @autor	Riku
 */

#ifndef _CAMERACOMPONENT_H_
#define _CAMERACOMPONENT_H_

#include "Component.h"
#include "../Engine.h"
#include "../VecMath.h"
#include "../GameEnum.h"
#include "../Camera/EventCamera.h"

class CameraComponent : public Component {
private:
	Vector3 mousePosition;			// マウスの位置
	Vector3 mouseMoveValue;			// マウスの移動量
	float sensitivity;				// 感度
	float playerDistancePos;		// プレイヤーから離れる距離
	GameEnum::CameraState state;	// カメラの状態

	const float CAMERA_ROTATION_X_MAX;		// カメラの角度のxの最大
	const float CAMERA_ROTATION_X_MIN;		// カメラの角度のxの最小
	const float PLAYER_HEAD_HEIGHT;			// プレイヤーの頭の高さ

public:
	EventCameraPtr eventCamera;		// イベントカメラ

public:
	CameraComponent();
	~CameraComponent() = default;

public:
	/*
	 *	最初のUpdateの直前に呼び出される処理
	 */
	void Start() override;
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;

public:
	/*
	 *	カメラの状態変更
	 */
	inline void SetState(const GameEnum::CameraState setState) { state = setState; };
	/*
	 *	カメラの状態取得
	 */
	inline GameEnum::CameraState GetState() const { return state; }
};
// 別名定義
using CameraComponentPtr = std::shared_ptr<CameraComponent>;

#endif // !_CAMERACOMPONENT_H_
