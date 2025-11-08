/*
 *	@file	RevolverArm.cpp
 *  @author Riku
 */

#include "RevolverArm.h"

void RevolverArm::ArmUpdate(float deltaTime, GameObject* player, Engine* engine) {
	// ¶ƒNƒŠƒbƒN‚ÅŽËŒ‚
	if (GetMouseInput() & MOUSE_INPUT_LEFT)
		ShotBullet(player->position, player->rotation);
}
