/*
 *	@file	AudioUtility.h
 *	@author	Seki
 */

#ifndef _AUDIO_UTILITY_H_
#define _AUDIO_UTILITY_H_

#include "AudioManager.h"

/*
 *	‰¹—ÊŠÖ˜AÀsˆ—
 */
class AudioUtility {
public:
	/*
	 *	BGM‚ÌÄ¶
	 */
	static void PlayBGM() {
		AudioManager::GetInstance().PlayBGM();
	}
	/*
	 *	BGM‚Ì’â~
	 */
	static void StopBGM() {
		AudioManager::GetInstance().StopBGM();
	}
	/*
	 *	BGM‚Ì•ÏX
	 *  @param[in]	const std::string& setKeyName	•ÏXæBGM–¼
	 */
	static void ChangeBGM(const std::string& setKeyName) {
		AudioManager::GetInstance().ChangeBGM(setKeyName);
	}
	/*
	 *	BGMƒnƒ“ƒhƒ‹‚Ì“o˜^
	 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éBGM–¼
	 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
	 */
	static void RegisterBGMHandle(const std::string& setKeyName, const int setHandle) {
		AudioManager::GetInstance().RegisterBGMHandle(setKeyName, setHandle);
	}

public:
	/*
	 *	SEÄ¶ˆ—
	 *  @param[in]	const std::string& setKeyName	Ä¶‚·‚éSE–¼
	 */
	static void PlaySE(const std::string& setKeyName, const int setVolume) {
		AudioManager::GetInstance().PlaySE(setKeyName);
	}
	/*
	 *	SEƒnƒ“ƒhƒ‹‚Ì“o˜^
	 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éSE–¼
	 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
	 */
	static void RegisterSE(const std::string& setKeyName, const int setHandle) {
		AudioManager::GetInstance().RegisterSEHandle(setKeyName, setHandle);
	}

public:
	/*
	 *	BGM‰¹—Ê‚Ìæ“¾
	 *  @return		int (0`100)
	 */
	static int GetBGMVolume() {
		return AudioManager::GetInstance().GetBGMVolume(); 
	}
	/*
	 *	BGM‰¹—Ê‚Ìİ’è
	 *  @param[in]	const int setVolume		‰¹—Ê (0`100)
	 */
	static void SetBGMVolume(const int setVolume) {
		AudioManager::GetInstance().SetBGMVolume(setVolume); 
	}
	/*
	 *	SE‰¹—Ê‚Ìæ“¾
	 *	@return		int (0`100)
	 */
	static int GetSEVolume() {
		return AudioManager::GetInstance().GetSEVolume(); 
	}
	/*
	 *	SE‰¹—Ê‚Ìİ’è
	 *	@param[in]	const int setVolume		‰¹—Ê (0`100)
	 */
	inline void SetSEVolume(const int setVolume) { 
		AudioManager::GetInstance().SetSEVolume(setVolume); 
	}
};
#endif // !_AUDIO_UTILITY_H_
