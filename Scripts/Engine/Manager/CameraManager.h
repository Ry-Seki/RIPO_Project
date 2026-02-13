/*
 *	@file	CameraManager.h
 *	@author	Riku
 */
#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_

#include "../Singleton.h"
#include "../Engine.h"
#include "../Component/CameraComponent.h"
#include "../GameEnum.h"

 /*
  *	カメラの管理クラス
  */
class CameraManager : public Singleton<CameraManager> {
	// フレンド宣言
	friend class Singleton<CameraManager>;

private:
	Engine* engine;
	GameObjectPtr camera;
	CameraComponentPtr cameraComponent;
	GameObjectPtr target;

private:
	CameraManager();
	~CameraManager() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize(Engine& setEngine);
	/*
	 *	カメラ生成
	 */
	void CreateCamera(const std::string& name,
		const Vector3& position = { 0.0f, 0.0f, 0.0f },
		const Vector3& rotation = { 0.0f, 0.0f, 0.0f });
	/*
	 *	カメラのリセット
	 *	@author	Seki
	 */
	void ResetCamera();

	/*
	 *	カメライベントの再生
	 */
	void CameraEventPlay(GameEnum::CameraEvent event);
	
public:
	/*
	 *	追従するターゲットをセット
	 */
	inline void SetTarget(GameObjectPtr setTarget) { target = setTarget; }
	/*
	 *	追従するターゲットをゲット
	 */
	inline GameObjectPtr GetTarget() const { return target; }
	/*
	 *	カメラのゲット
	 */
	inline GameObjectPtr GetCamera() const { return camera; }
	/*
	 *	カメラの状態取得
	 */
	inline GameEnum::CameraState GetCameraState() const { return cameraComponent->GetState(); }
};

#endif // !_CAMERAMANAGER_H_
