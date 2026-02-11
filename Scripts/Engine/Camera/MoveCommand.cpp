/*
 *	@file	MoveCommand.cpp
 */

#include "MoveCommand.h"

/*
 *	コマンド開始時に呼ばれる関数
 */
void MoveCommand::Start(GameObject* camera) {
	CameraCommand::Start(camera);
	startPos = camera->position;
}

/*
 *	コマンドの更新処理
 */
bool MoveCommand::Update(GameObject* camera, float deltaTime) {
	
}
