/*
 *	@file	BGMSource.cpp
 *	@author	Seki
 */

#include "BGMSource.h"

/*
 *	@brief		‰¹Œ¹‚ÌÄ¶ˆ—
 *  @param[in]	float setVolume		‰¹—Ê
 */
void BGMSource::Play(float setVolume) {
	ChangeAudioVolume(setVolume);
	PlaySoundMem(audioHandle, DX_PLAYTYPE_LOOP);
}
/*
 *	@brief		‰¹Œ¹‚Ì’â~ˆ—
 */
void BGMSource::Stop() {
	StopSoundMem(audioHandle);
}
/*
 *	@brief		‰¹Œ¹‚Ì”jŠüˆ—
 */
void BGMSource::Teardown() {
	DeleteSoundMem(audioHandle);
	audioHandle = -1;
}
