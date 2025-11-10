/*
 *	@file	Vision.cpp
 *  @author kuu
 */
#include "Vision.h"

/*
 *	Ž‹ŠEˆ—
 *	param[in]	const Vector3& beholderPos	Ž©g‚Ìposition
 *	param[in]	const Vector3& beholderDir	Ž©g‚Ìrotation
 *	param[in]	const Vector3& targetPos	–Ú•W‚Ìposition
 *	param[in]	float viewAngle				Ž‹–ìŠp‚Ì”¼Šp
 *	param[in]	float viewDistance			Ž‹ŠE‚Ì‹——£
 */
bool Vision(const Vector3& beholderPos, const Vector3& beholderDir, const Vector3& targetPos, float viewAngle, float viewDistance) {
	// Ž©g‚Æ–Ú•W‚Ì‹——£
	float distance = Distance(targetPos, beholderPos);
	// Ž‹ŠE‹——£”»’è
	if (distance > viewDistance) return false;
	// Ž©g‚Ì•ûŒüƒxƒNƒgƒ‹
	Vector3 direction = ForwardDir(beholderDir);
	// –Ú•W‚Ì•ûŒü
	Vector3 toTargetDir = Direction(beholderPos, targetPos);
	// Ž‹–ìŠp‚ÌŒvŽZ
	float dot = Clamp(Dot(direction, toTargetDir), -1.0f, 1.0f);
	float angle = acosf(dot);
	viewAngle = viewAngle * Deg2Rad;
	if (angle > viewAngle) {
		return false;
	}
	// “–‚½‚Á‚Ä‚¢‚é
	return true;
}