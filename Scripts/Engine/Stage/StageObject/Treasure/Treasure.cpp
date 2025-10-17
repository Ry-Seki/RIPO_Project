/*
 *	@file	@Treasure.cpp
 *  @author	oorui
 */

#include "Treasure.h"

 Treasure::Treasure()  : StageObjectBase(){
 	modelHandle = -1;
 }
 
 Treasure::~Treasure() {
	
 }

 /*
   *	@function	ModelLoad
   *  @brief		ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
   *  @param		const int modelHandleBase
   */
void Treasure::ModelLoad(const int modelHandleBase) {
	modelHandle = modelHandleBase;
}


/*
 *	@function	Update
 *  @brief		XV
 *  @param		float deltaTime
 */
void Treasure::Update(float deltaTime) {


}
