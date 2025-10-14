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
	
private:
	CameraManager();
	~CameraManager() = default;

public:


};

#endif // !_CAMERAMANAGER_H_
