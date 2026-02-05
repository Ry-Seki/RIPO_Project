/*
 *	@file	SESource.h
 *	@author	Seki
 */

#ifndef _SE_SOURCE_H_
#define _SE_SOURCE_H_

#include "../AudioSource.h"
#include <DxLib.h>

/*
 *	@brief	SE音源クラス
 */
class SESource : public AudioSource {
public:
	/*
	 *	@brief	コンストラクタ
	 */
	SESource() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~SESource() override {}

public:
	/*
	 *	@brief		音源の再生処理
	 *  @param[in]	float setVolume		音量
	 */
	void Play(float setVolume) override;
	/*
	 *	@brief		音源の停止処理
	 */
	void Stop() override;
	/*
	 *	@brief		音源の破棄処理
	 */
	void Teardown() override;
};
#endif // !_SE_SOURCE_H_
