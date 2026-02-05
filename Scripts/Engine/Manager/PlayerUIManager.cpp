/*
 *	@file	PlayerUIManager.cpp
 *	@author	Riku
 */

#include "PlayerUIManager.h"

 /*
 *	‰Šú‰»
 */
void PlayerUIManager::Initialize() {
	reticle->Initialize();
}

/*
 *	•`‰æ
 */
void PlayerUIManager::Render() {
	reticle->Render();
}
