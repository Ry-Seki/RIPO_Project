/*
 *	@file	CameraCommand.h
 *	@author Riku
 */

#ifndef _CAMERACOMMAND_H_
#define _CAMERACOMMAND_H_

#include "../GameObject.h"

/*
 *	イベントカメラのコマンドの基底クラス
 */
class CameraCommand {
protected:
	float durationTime = 0.0f;	// 全体時間
	float elapsedTime = 0.0f;	// 経過時間

public:
	/* 
	 *	Commandの状態
	 */
	enum class CommandState {
		Waiting,	// 待機中
		Playing,	// 再生中
		Finished,	// 終了済み
	};
	CommandState state = CommandState::Waiting;

public:
	CameraCommand() = default;
	virtual ~CameraCommand() = default;

public:
	/*
	 *	コマンド開始時に呼ばれる関数
	 */
	virtual void Start(GameObject* camera) {
		elapsedTime = 0.0f;
		state = CommandState::Playing;
	}

	/*
	 *	コマンドの更新処理
	 */
	virtual void Update(GameObject* camera, float deltaTime) = 0;
};
using CameraCommandPtr = std::shared_ptr<CameraCommand>;
using CameraCommandList = std::vector<CameraCommandPtr>;

#endif // !_CAMERACOMMAND_H_
