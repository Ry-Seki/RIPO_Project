/*
 *	@file	VisionComponent.cpp
 *  @author kuu
 */
#include "VisionComponent.h"

VisionComponent::VisionComponent()
{
}

/*
  *	XVˆ—
  */
void VisionComponent::Update(float deltaTime) {

}

/*
 *	‹ŠEˆ—
 *	param[in]	const Vector3& beholderPos	©g‚Ìposition
 *	param[in]	const Vector3& beholderDir	©g‚Ìrotation
 *	param[in]	const Vector3& targetPos	–Ú•W‚Ìposition
 *	param[in]	float viewAngle				‹–ìŠp‚Ì”¼Šp
 *	param[in]	float viewDistance			‹ŠE‚Ì‹——£
 */
bool VisionComponent::Vision(const Vector3& beholderPos, const Vector3& beholderDir, const Vector3& targetPos, float viewAngle, float viewDistance) {
	// ©g‚Æ–Ú•W‚Ì‹——£
	float distance = Distance(targetPos, beholderPos);
	// ‹ŠE‹——£”»’è
	if (distance > viewDistance) return false;
	// ©g‚Ì•ûŒüƒxƒNƒgƒ‹
	Vector3 direction = ForwardDir(beholderDir);
	// –Ú•W‚Ì•ûŒü
	Vector3 toTargetDir = Offset(targetPos, beholderPos).Normalized();
	// ‹–ìŠp‚ÌŒvZ
	float angle = acosf(Dot(direction, toTargetDir) * Rad2Deg);
	if (angle > viewAngle) {
		return false;
	}
	return true;

}
