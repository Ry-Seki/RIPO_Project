/*
 *	@file	RotateCommand.h
 *	@author Riku
 */

#ifndef _ROTATECOMMAND_H_
#define _ROTATECOMMAND_H_

#include "CameraCommand.h"

/*
 *	カメラの回転コマンド
 */
class RotateCommand : public CameraCommand {
private:
	Vector3 startRot;	// 開始角度
	Vector3 targetRot;	// 回転先

public:
	RotateCommand(float setDurationTime, Vector3 setStartRot, Vector3 setTargetRot);
	virtual ~RotateCommand() = default;

public:
	/*
	 *	コマンド開始時に呼ばれる関数
	 */
	void Start(GameObject* camera) override;

	/*
	 *	コマンドの更新処理
	 */
	void Update(GameObject* camera, float deltaTime) override;

};

#endif // !_ROTATECOMMAND_H_
