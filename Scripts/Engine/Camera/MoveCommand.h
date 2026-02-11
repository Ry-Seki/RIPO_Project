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

public:
	/*
	 *	コマンド開始時に呼ばれる関数
	 */
	void Start(GameObject* camera) override;

	/*
	 *	コマンドの更新処理
	 */
	bool Update(GameObject* camera, float deltaTime) override;

};

#endif // !_MOVECOMMAND_H_
