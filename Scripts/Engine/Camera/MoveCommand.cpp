/*
 *	@file	MoveCommand.cpp
 */

#include "MoveCommand.h"

MoveCommand::MoveCommand(float setDurationTime, Vector3 setTargetPos) {
	durationTime = setDurationTime;
	targetPos = setTargetPos;
	state = CommandState::Waiting;
}

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
void MoveCommand::Update(GameObject* camera, float deltaTime) {
	if (durationTime == 0)
		state = CommandState::Finished;
	
	// 経過時間保存
	elapsedTime += deltaTime;

	// 経過時間の割合で位置を決定
	float rate = Clamp(elapsedTime / durationTime, 0, 1);
	camera->position = Lerp(startPos, targetPos, rate);

	if (elapsedTime >= durationTime)
		state = CommandState::Finished;
}
