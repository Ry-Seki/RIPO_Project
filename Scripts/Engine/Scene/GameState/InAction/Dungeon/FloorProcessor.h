/*
 *	@file	FloorProcessor.h
 *	@author	Seki
 */

#ifndef _FLOOR_PROCESSOR_H_
#define _FLOOR_PROCESSOR_H_

/*
 *	@brief	ダンジョンのフロアを管理するクラス
 */
class FloorProcessor {

public:
	/*
	 *	@brief	現在のフロアの片付け処理
	 */
	void TeardownCurrentFloor();
	/*
	 *	@brief	次のフロアの準備処理
	 */
	void SetupNextFloor();
};

#endif // !_FLOOR_PROCESSOR_H_