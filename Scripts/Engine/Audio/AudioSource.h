/*
 *	@file	AudioSource.h
 *	@author	Seki
 */

#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include "../VecMath.h"
#include "../GameConst.h"

#include <memory>
#include <algorithm>

/*
 *	@brief	音源の基底クラス
 */
class AudioSource {
protected:
	int audioHandle = -1;
	
public:
	/*
	 *	@brief	デストラクタ
	 */
	virtual ~AudioSource() { Teardown(); }

public:
	/*
	 *	@brief		音源の再生処理
	 *  @param[in]	float setVolume		音量
	 */
	virtual void Play(float setVolume) = 0;
	/*
	 *	@brief		音源の停止処理
	 */
	virtual void Stop() = 0;
	/*
	 *	@brief		片付け処理
	 */
	virtual void Teardown();

public:
	/*
	 *	@brief		音源ハンドルの設定
	 *  @param[in]	int setHandle	音源ハンドル
	 */
	void SetAudioHandle(int setHandle);
	/*
	 *	@brief		再生中かどうかを判別する
	 *  @return		bool
	 */
	bool IsPlaying() const;
	/*
	 *	@brief		音量の変更
	 *  @param[in]	float setVolume		設定する音量(0.0f～1.0f)
	 */
	void ChangeAudioVolume(float setVolume);

public:
	/*
	 *	音源ハンドルの取得
	 *  @return		int
	 */
	inline int GetAudioHandle() const { return audioHandle; }
};
// 別名定義
using AudioSourcePtr = std::shared_ptr<AudioSource>;

#endif // !_AUDIO_SOURCE_H_

