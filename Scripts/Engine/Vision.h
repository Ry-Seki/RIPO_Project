/*
 *	@file	Vision.h
 *  @author kuu
 */
#ifndef _VISION_H_
#define _VISION_H_

#include "Collision/Collision.h"
#include "GameObject.h"

/*
 *	‹ŠEˆ—
 *	param[in]	const GameObjectPtr beholder©g
 *	param[in]	const Vector3& direction	‹ŠE‚Ì•ûŒüƒxƒNƒgƒ‹
 *	param[in]	const Vector3& targetPos	–Ú•W‚ÌˆÊ’u
 *	param[in]	float viewAngle				‹–ìŠp‚Ì”¼Šp
 *	param[in]	float viewDistance			‹ŠE‚Ì‹——£
 */
bool Vision(const GameObject* beholder, const Vector3& direction, const Vector3& targetPos, float viewAngle, float viewDistance);

#endif // !_VISION_H_
