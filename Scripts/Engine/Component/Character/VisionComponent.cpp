/*
 *	@file	VisionComponent.cpp
 *  @author kuu
 */
#include "VisionComponent.h"

VisionComponent::VisionComponent()
{
}

/*
  *	更新処理
  */
void VisionComponent::Update(float deltaTime) {

}

/*
 *	視界処理
 *	param[in]	const Vector3& beholderPos	自身のposition
 *	param[in]	const Vector3& beholderDir	自身のrotation
 *	param[in]	const Vector3& targetPos	目標のposition
 *	param[in]	float viewAngle				視野角の半角
 *	param[in]	float viewDistance			視界の距離
 */
bool VisionComponent::Vision(const Vector3& beholderPos, const Vector3& beholderDir, const Vector3& targetPos, float viewAngle, float viewDistance) {
	// 自身と目標の距離
	float distance = Distance(targetPos, beholderPos);
	// 視界距離判定
	if (distance > viewDistance) return false;
	// 自身の方向ベクトル
	//Vector3 direction = 
	//float angle = 
}
