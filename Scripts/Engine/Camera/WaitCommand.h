/*
 *	@file	WaitCommand.h
 *	@author Riku
 */

#ifndef _WAITCOMMAND_H_
#define _WAITCOMMAND_H_

#include "CameraCommand.h"

 /*
  *	カメラの待機コマンド
  */
class WaitCommand : public CameraCommand {
private:

public:
	WaitCommand(float setDurationTime);
	virtual ~WaitCommand() = default;

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

#endif // !_WAITCOMMAND_H_
