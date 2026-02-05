/*
 *	@file	BGMSource.h
 *	@author	Seki
 */

#ifndef _BGM_SOURCE_H_
#define _BGM_SOURCE_H_

#include "../AudioSource.h"

/*
 *	@brief	BGM音源クラス
 */
class BGMSource : public AudioSource {
public:
	/*
	 *	@brief	コンストラクタ
	 */
	BGMSource() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~BGMSource() override {}

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
#endif // !_BGM_SOURCE_H_
