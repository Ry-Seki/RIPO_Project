/*
 *	@file	BGMSource.h
 *	@author	Seki
 */

#ifndef _BGM_SOURCE_H_
#define _BGM_SOURCE_H_

#include "../AudioSource.h"

/*
 *	BGM音源クラス
 */
class BGMSource : public AudioSource {
public:
	/*
	 *	コンストラクタ
	 */
	BGMSource() = default;
	/*
	 *	デストラクタ
	 */
	~BGMSource() override {}

public:
	/*
	 *	音源の再生処理
	 *  @param[in]	int setVolume	音量
	 */
	void Play(int setVolume) override;
	/*
	 *	音源の停止処理
	 */
	void Stop() override;
	/*
	 *	音源の破棄処理
	 */
	void Teardown() override;
};
#endif // !_BGM_SOURCE_H_
