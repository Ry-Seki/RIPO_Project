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
	float durationTime;	// 全体時間
	float elapsedTime;	// 経過時間
	bool isFinished;	// 終了フラグ

public:
	CameraCommand() = default;
	virtual ~CameraCommand() = default;

public:
	/*
	 *	コマンド開始時に呼ばれる関数
	 */
	virtual void Start(GameObject* camera) {
		elapsedTime = 0.0f;
		isFinished = false;
	}

	/*
	 *	コマンドの更新処理
	 */
	virtual bool Update(GameObject* camera, float deltaTime) = 0;

};

#endif // !_CAMERACOMMAND_H_
