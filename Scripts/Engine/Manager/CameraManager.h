/*
 *	@file	CameraManager.h
 *	@author	Riku
 */
#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include "../Component/CameraComponent.h"

 /*
  *	カメラの管理クラス
  */
class CameraManager : public Singleton<CameraManager> {
	// フレンド宣言
	friend class Singleton<CameraManager>;
private:
	Engine* engine;
	GameObjectPtr camera;
	GameObjectPtr target;

private:
	CameraManager();
	~CameraManager() = default;

public:
	void CreateCamera(const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });
	inline GameObjectPtr GetCamera() const { return camera; }
	inline void SetTarget(GameObjectPtr setTarget) { target = setTarget; }
	inline GameObjectPtr GetTarget() const { return target; }

};

#endif // !_CAMERAMANAGER_H_
