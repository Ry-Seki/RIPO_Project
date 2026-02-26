/*
 *	@file	WaitCommand.cpp
 *	@author Riku
 */

#include "WaitCommand.h"

WaitCommand::WaitCommand(float setDurationTime) {
	durationTime = setDurationTime;
	state = CommandState::Waiting;
}

/*
 *	コマンド開始時に呼ばれる関数
 */
void WaitCommand::Start(GameObject* camera) {
	CameraCommand::Start(camera);
}

/*
 *	コマンドの更新処理
 */
void WaitCommand::Update(GameObject* camera, float deltaTime) {
	if (durationTime == 0)
		state = CommandState::Finished;

	// 経過時間保存
	elapsedTime += deltaTime;

	if (elapsedTime >= durationTime)
		state = CommandState::Finished;
}
