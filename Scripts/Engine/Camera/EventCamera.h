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

class EventCamera {
private:
	GameEnum::CameraEvent eventNum;
	std::unordered_map<GameEnum::CameraEvent, CameraCommandList> events;
	int commandCount;

public:
	EventCamera();
	~EventCamera() = default;

public:
	/*
	 *	‰Šú‰»ˆ—
	 */
	void Initialize(GameObject* camera, GameEnum::CameraEvent setEvent);

	/*
	 *	XVˆ—
	 */
	void Update(GameObject* camera, float deltaTime);
};
// •Ê–¼’è‹`
using EventCameraPtr = std::shared_ptr<EventCamera>;

#endif // !_EVENTCAMERA_H_
