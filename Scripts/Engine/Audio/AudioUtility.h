/*
 *	@file	AudioUtility.h
 *	@author	Seki
 */

#ifndef _AUDIO_UTILITY_H_
#define _AUDIO_UTILITY_H_

#include "AudioManager.h"

/*
 *	@brief	‰¹—ÊŠÖ˜AÀsˆ—
 *  @note	–¼‘O‹óŠÔ‚Å‚ÌÈ—ª‰Â”\	using namespace AudioUtility
 */
namespace AudioUtility {
	/*
	 *	BGM‚ÌÄ¶
	 */
	inline void PlayBGM() {
		AudioManager::GetInstance().PlayBGM();
	}
	/*
	 *	BGM‚Ì’â~
	 */
	inline void StopBGM() {
		AudioManager::GetInstance().StopBGM();
	}
	/*
	 *	@brief		BGM‚Ì•ÏX
	 *  @param[in]	const std::string& setKeyName	•ÏXæBGM–¼
	 */
	inline void ChangeBGM(const std::string& setKeyName) {
		AudioManager::GetInstance().ChangeBGM(setKeyName);
	}
	/*
	 *	@brief		BGMƒnƒ“ƒhƒ‹‚Ì“o˜^
	 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éBGM–¼
	 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
	 */
	inline void RegisterBGMHandle(const std::string& setKeyName, const int setHandle) {
		AudioManager::GetInstance().RegisterBGMHandle(setKeyName, setHandle);
	}

	/*
	 *	@brief		SEÄ¶ˆ—
	 *  @param[in]	const std::string& setKeyName	Ä¶‚·‚éSE–¼
	 */
	inline void PlaySE(const std::string& setKeyName) {
		AudioManager::GetInstance().PlaySE(setKeyName);
	}
	/*
	 *	@brief		SEƒnƒ“ƒhƒ‹‚Ì“o˜^
	 *	@param[in]	const std::string&	setKeyName	“o˜^‚·‚éSE–¼
	 *	@param[in]	const int setHandle				“o˜^‚·‚é‰¹Œ¹ƒnƒ“ƒhƒ‹
	 */
	inline void RegisterSEHandle(const std::string& setKeyName, const int setHandle) {
		AudioManager::GetInstance().RegisterSEHandle(setKeyName, setHandle);
	}

	/*
	 *	@brief		ƒ}ƒXƒ^[‰¹—Ê‚Ìæ“¾
	 *  @return		float (0.0f`1.0f)
	 */
	inline float GetMasterVolume() {
		return AudioManager::GetInstance().GetMasterVolume();
	}
	/*
	 *	@brief		ƒ}ƒXƒ^[‰¹—Ê‚Ìİ’è
	 *  @param[in]	float setVolume		‰¹—Ê (0.0f`1.0f)
	 */
	inline void SetMasterVolume(float setVolume) {
		return AudioManager::GetInstance().SetMasterVolume(setVolume);
	}
	/*
	 *	@brief		BGM‰¹—Ê‚Ìæ“¾
	 *  @return		float (0.0f`1.0f)
	 */
	inline float GetBGMVolume() {
		return AudioManager::GetInstance().GetBGMVolume(); 
	}
	/*
	 *	@brief		BGM‰¹—Ê‚Ìİ’è
	 *  @param[in]	float setVolume		‰¹—Ê (0.0f`1.0f)
	 */
	inline void SetBGMVolume(float setVolume) {
		AudioManager::GetInstance().SetBGMVolume(setVolume); 
	}
	/*
	 *	@brief		SE‰¹—Ê‚Ìæ“¾
	 *	@return		float (0.0f`1.0f)
	 */
	inline float GetSEVolume() {
		return AudioManager::GetInstance().GetSEVolume(); 
	}
	/*
	 *	@brief		SE‰¹—Ê‚Ìİ’è
	 *	@param[in]	float setVolume		‰¹—Ê (0.0f`1.0f)
	 */
	inline void SetSEVolume(float setVolume) { 
		AudioManager::GetInstance().SetSEVolume(setVolume); 
	}
};
#endif // !_AUDIO_UTILITY_H_
