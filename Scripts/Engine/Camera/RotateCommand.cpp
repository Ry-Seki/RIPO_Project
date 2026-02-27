/*
 *	@file	RotateCommand.cpp
 *	@author Riku
 */

#include "RotateCommand.h"

RotateCommand::RotateCommand(float setDurationTime, Vector3 setStartRot, Vector3 setTargetRot) {
	durationTime = setDurationTime;
	startRot = setStartRot;
	targetRot = setTargetRot;
	state = CommandState::Waiting;
}

/*
 *	コマンド開始時に呼ばれる関数
 */
void RotateCommand::Start(GameObject* camera) {
	CameraCommand::Start(camera);
}

/*
 *	コマンドの更新処理
 */
void RotateCommand::Update(GameObject* camera, float deltaTime) {
	if (durationTime == 0)
		state = CommandState::Finished;

	// 経過時間保存
	elapsedTime += deltaTime;

	// 経過時間の割合で位置を決定
	float rate = Clamp(elapsedTime / durationTime, 0, 1);
	camera->rotation = Lerp(startRot, targetRot, rate);

	if (elapsedTime >= durationTime)
		state = CommandState::Finished;
}
