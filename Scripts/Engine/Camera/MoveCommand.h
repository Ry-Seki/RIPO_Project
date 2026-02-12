/*
 *	@file	MoveCommand.h
 */

#ifndef _MOVECOMMAND_H_
#define _MOVECOMMAND_H_

#include "CameraCommand.h"

/*
 *	カメラの移動コマンド
 */
class MoveCommand : public CameraCommand {
private:
	Vector3 startPos;	// 開始位置
	Vector3 targetPos;	// 移動先

public:
	MoveCommand(float setDurationTime, Vector3 setTargetPos);
	virtual ~MoveCommand() = default;

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

#endif // !_MOVECOMMAND_H_
