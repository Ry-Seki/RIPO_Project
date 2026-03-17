/*
 *	@file	Sprite.cpp
 *	@author	Seki
 */

#include "Sprite.h"

#include <DxLib.h>

/*
 *	@brief	€”õ‘Oˆ—
 */
void Sprite::Setup() {
	currentFrame = 0;
	isVisible = true;
}
/*
 *	@brief	•`‰æˆ—
 */
void Sprite::Render() {
	if (!isVisible) return;
	if (currentFrame < 0 || graphHandleList.empty()) return;

	DrawGraph(rect.x, rect.y, graphHandleList[currentFrame], TRUE);
}
