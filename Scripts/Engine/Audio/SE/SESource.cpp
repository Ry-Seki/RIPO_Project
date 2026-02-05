/*
 *	@file	SESource.cpp
 *	@author	Seki
 */

#include "SESource.h"

/*
 *	@brief		‰¹Œ¹‚ÌÄ¶ˆ—
 *  @param[in]	float setVolume		‰¹—Ê
 */
void SESource::Play(float setVolume) {
	ChangeAudioVolume(setVolume);
	PlaySoundMem(audioHandle, DX_PLAYTYPE_BACK);
}
/*
 *	@brief		‰¹Œ¹‚Ì’â~ˆ—
 */
void SESource::Stop() {
}
/*
 *	@brief		‰¹Œ¹‚Ì”jŠüˆ—
 */
void SESource::Teardown() {
	DeleteSoundMem(audioHandle);
	audioHandle = -1;
}
