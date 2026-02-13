/*
 *	@file	EventCamera.h
 *	@author	Riku
 */

#ifndef _EVENTCAMERA_H_
#define _EVENTCAMERA_H_

#include "../GameObject.h"
#include "../GameEnum.h"
#include "CameraCommand.h"
#include <unordered_map>
#include <functional>

class EventCamera {
private:
	GameEnum::CameraEvent eventNum;
	int commandCount;

	struct Event {
		CameraCommandList commands;			// イベントを構成するコマンド群
		std::function<void()> onFinished;	// 終了時処理
	};
	std::unordered_map<GameEnum::CameraEvent, Event> events;

public:
	EventCamera();
	~EventCamera() = default;

public:
	/*
	 *	初期化処理
	 */
	void Initialize(GameObject* camera, GameEnum::CameraEvent setEvent);

	/*
	 *	更新処理
	 */
	void Update(GameObject* camera, float deltaTime);
};
// 別名定義
using EventCameraPtr = std::shared_ptr<EventCamera>;

#endif // !_EVENTCAMERA_H_
