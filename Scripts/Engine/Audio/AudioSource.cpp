/*
 *	@file	AudioSource.cpp
 *	@author	Seki
 */

#include "AudioSource.h"

#include <DxLib.h>

/*
 *	@brief		片付け処理
 */
void AudioSource::Teardown() {
	if (audioHandle != -1) DeleteSoundMem(audioHandle);
}
/*
 *	@brief		音源ハンドルの設定
 *  @param[in]	int setHandle	音源ハンドル
 */
void AudioSource::SetAudioHandle(int setHandle) {
	audioHandle = DuplicateSoundMem(setHandle);
}
/*
 *	@brief		再生中かどうかを判別する
 *  @return		bool
 */
bool AudioSource::IsPlaying() const {
	return CheckSoundMem(audioHandle);
}
/*
 *	@brief		音量の変更
 *  @param[in]	float setVolume		設定する音量(0.0f～1.0f)
 */
void AudioSource::ChangeAudioVolume(float setVolume) {
	setVolume = std::clamp(setVolume, 0.0f, 1.0f);
	int dxVolume = static_cast<int>(setVolume * GameConst::MAX_DXLIB_VOLUME);
	ChangeVolumeSoundMem(dxVolume, audioHandle);
}
