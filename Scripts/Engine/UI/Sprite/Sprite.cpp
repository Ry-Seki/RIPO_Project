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
	currentIndex = 0;
}
/*
 *	@brief	•`‰æˆ—
 */
void Sprite::Render() {
	if (currentIndex < 0 || graphHandleList.empty()) return;

	DrawGraph(rect.x, rect.y, graphHandleList[currentIndex], TRUE);
}
