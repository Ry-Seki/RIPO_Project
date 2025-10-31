/*
 *	@file	BGMSource.h
 *	@author	Seki
 */

#include "BGMSource.h"

/*
 *	‰¹Œ¹‚ÌÄ¶
 *  @param[in]	int setVolume	‰¹—Ê
 */
void BGMSource::Play(int setVolume) {
	int volume = ChangeVolumeCorrection(setVolume);
	PlaySoundMem(audioHandle, DX_PLAYTYPE_LOOP);
}
/*
 *	‰¹Œ¹‚Ì’â~
 */
void BGMSource::Stop() {
	StopSoundMem(audioHandle);
}
/*
 *	‰¹Œ¹‚Ì”jŠüˆ—
 */
void BGMSource::Teardown() {
	DeleteSoundMem(audioHandle);
	audioHandle = -1;
}
