/*
 *	@file	SESource.h
 *	@author	Seki
 */

#ifndef _SE_SOURCE_H_
#define _SE_SOURCE_H_

#include "../AudioSource.h"
#include <DxLib.h>

/*
 *	SE音源クラス
 */
class SESource : public AudioSource {
public:
	/*
	 *	コンストラクタ
	 */
	SESource() = default;
	/*
	 *	デストラクタ
	 */
	~SESource() override {}

public:
	/*
	 *	音源の再生
	 *  @param[in]	int setVolume	音量
	 */
	void Play(int setVolume) override;
	/*
	 *	音源の停止
	 */
	void Stop() override;
	/*
	 *	音源の破棄処理
	 */
	void Teardown() override;
};
#endif // !_SE_SOURCE_H_
