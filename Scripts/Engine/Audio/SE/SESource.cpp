/*
 *	@file	SESource.cpp
 *	@author	Seki
 */

#include "SESource.h"

/*
 *	‰¹Œ¹‚ÌÄ¶ˆ—
 *  @param[in]	int setVolume	‰¹—Ê
 */
void SESource::Play(int setVolume) {
	int volume = ChangeVolumeCorrection(setVolume);
	PlaySoundMem(audioHandle, DX_PLAYTYPE_BACK);
}
/*
 *	‰¹Œ¹‚Ì’â~ˆ—
 */
void SESource::Stop() {
}
/*
 *	‰¹Œ¹‚Ì”jŠüˆ—
 */
void SESource::Teardown() {
	DeleteSoundMem(audioHandle);
	audioHandle = -1;
}
