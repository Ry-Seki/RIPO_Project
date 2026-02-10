/*
 *	@file	EventCamera.h
 *	@author	Riku
 */

#ifndef _EVENTCAMERA_H_
#define _EVENTCAMERA_H_

#include "../GameObject.h"
#include <memory>

class EventCamera {
public:
	EventCamera() = default;
	~EventCamera() = default;

public:
	/*
	 *	‰Šú‰»ˆ—
	 */
	void Initialize();

	/*
	 *	XVˆ—
	 */
	void Update(GameObject* camera, float deltaTime);
};
// •Ê–¼’è‹`
using EventCameraPtr = std::shared_ptr<EventCamera>;

#endif // !_EVENTCAMERA_H_
