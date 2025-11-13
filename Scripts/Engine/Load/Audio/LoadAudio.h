/*
 *	@file	LoadAudio.h
 *	@author	Seki
 */

#ifndef _LOAD_AUDIO_H_
#define _LOAD_AUDIO_H_

#include "../LoadBase.h"

#include <DxLib.h>

/*
 *	音源の読み込みクラス
 */
class LoadAudio : public LoadBase {
	int audioHandle = -1;	// 音源ハンドル

public:
	/*
	 *	コンストラクタ
	 *	param[in]	const std::string& setFilePath		ファイルパス
	 */
	explicit LoadAudio(const std::string& setFilePath) : LoadBase(setFilePath) {}

public:
	/*
	 *	読み込み処理
	 */
	void Load() override;

public:
	/*
	 *	モデルハンドルの取得
	 *	return int
	 */
	inline int GetHandle() const { return audioHandle; }
};
#endif // !_LOAD_AUDIO_H_
