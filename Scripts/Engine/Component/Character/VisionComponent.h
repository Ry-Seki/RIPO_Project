/*
 *	@file	VisionComponent
 *  @author kuu
 */
#ifndef _VISIONCOMPONENT_H_
#define _VISIONCOMPONENT_H_

#include "../Component.h"
#include "../../RayCast.h"

/*
 *	‹ŠE”»’èƒNƒ‰ƒX
 */
class VisionComponent : public Component {
private:
	//Engine* engine;

public:
	VisionComponent() = default;
	~VisionComponent() = default;

public:

	/*
	 *	‹ŠEˆ—
	 *	param[in]	const Vector3& beholderPos	©g‚ÌˆÊ’u
	 *	param[in]	const Vector3& beholderDir	©g‚ÌŒü‚«
	 *	param[in]	const Vector3& targetPos	–Ú•W‚ÌˆÊ’u
	 *	param[in]	float viewAngle				‹–ìŠp‚Ì”¼Šp
	 *	param[in]	float viewDistance			‹ŠE‚Ì‹——£
	 */
	bool Vision(const Vector3& beholderPos, const Vector3& beholderDir, const Vector3& targetPos, float viewAngle, float viewDistance);



};


#endif // !_VISIONCOMPONENT_H_
