/*
 *	@file	AudioSource.h
 *	@author	Seki
 */

#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include "DxLib.h"
#include "../VecMath.h"
#include "../GameConst.h"

#include <memory>

/*
 *	音源の基底クラス
 */
class AudioSource {
protected:
	int audioHandle;
	
public:
	/*
	 *	デストラクタ
	 */
	virtual ~AudioSource() { Teardown(); }

public:
	/*
	 *	音源の再生処理
	 *  @param[in]	int setVolume	音量
	 */
	virtual void Play(int setVolume) = 0;
	/*
	 *	音源の停止処理
	 */
	virtual void Stop() = 0;
	/*
	 *	音源の破棄処理
	 */
	virtual void Teardown() = 0;

public:
	/*
	 *	音源ハンドルの取得
	 *  @return		int
	 */
	inline int GetAudioHandle() const { return audioHandle; }
	/*
	 *	音源ハンドルの設定
	 *  @param[in]	int setHandle	音源ハンドル
	 */
	inline virtual void SetAudioHandle(int setHandle) { audioHandle = DuplicateSoundMem(setHandle); }
	/*
	 *	再生中かどうかを判別する
	 *  @return		bool
	 */
	inline bool IsPlaying() const { return CheckSoundMem(audioHandle); }
	/*
	 *	音量の設定
	 *  @param[in]	int setVolume	設定する音量(0～100)
	 */
	inline void ChangeAudioVolume(int setVolume) { 
		// 音量の数値の変換(0～100)->(0～255)
		int volume = ChangeVolumeCorrection(setVolume);
		ChangeVolumeSoundMem(volume, audioHandle);
	}
	/*
	 *	音量の数値の変換(0～100)->(0～255)
	 *  @param[in]	int setVolume	設定する音量(0～100)
	 *  @return		int
	 */
	inline int ChangeVolumeCorrection(int setVolume) {
		// 最大値、最小値を固定
		int volume = Clamp(static_cast<float>(setVolume), GameConst::_MIN_VOLUME, GameConst::_MAX_DXLIB_VOLUME);
		// 音量の数値の変換(0～100)->(0～255)
		volume *= GameConst::_MAX_DXLIB_VOLUME / GameConst::_MAX_VOLUME;
		return volume;
	}
};
// 別名定義
using AudioSourcePtr = std::shared_ptr<AudioSource>;

#endif // !_AUDIO_SOURCE_H_

