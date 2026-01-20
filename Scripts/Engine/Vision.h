/*
 *	@file	Vision.h
 *  @author kuu
 */
#ifndef _VISION_H_
#define _VISION_H_

#include "Collision/Collision.h"

/*
 *	視界処理
 *	param[in]	const Vector3& beholderPos	自身の位置
 *	param[in]	const Vector3& direction	視界の方向ベクトル
 *	param[in]	const Vector3& targetPos	目標の位置
 *	param[in]	float viewAngle				視野角の半角
 *	param[in]	float viewDistance			視界の距離
 */
bool Vision(const Vector3& beholderPos, const Vector3& direction, const Vector3& targetPos, float viewAngle, float viewDistance);

#endif // !_VISION_H_
