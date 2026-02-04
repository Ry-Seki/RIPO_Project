/*
 *	@file	EffectComponent.cpp
 *  @author oorui
 */

#include "EffectComponent.h"
#include <EffekseerForDxLib.h>
 /*
  * コンストラクタ
  */
EffectComponent::EffectComponent(int& _resourceHandle, bool isLoop)
	:resourceHandle(_resourceHandle)
	, playingHandle(-1)
	, isLoop(isLoop)
	, isVisible(false) {
}

/*
 *	エフェクト再生
 *  @tips	ループの場合でも使用可能
 */
void EffectComponent::EffectRenderer() {
	if (!isVisible)return;

	// まだ再生ハンドルを持っていなければ再生開始
	if (playingHandle == -1) {
		playingHandle = PlayEffekseer3DEffect(resourceHandle);
		
	}

	// 再生が終了していれば
	if (!IsEffekseer3DEffectPlaying(playingHandle)) {

		// ループ再生
		if (isLoop) {
			playingHandle = PlayEffekseer3DEffect(resourceHandle);
		}
		// ループしない
		else {
			isVisible = false;
			// 再利用防止
			playingHandle = -1;
			return;
		}
	}


	// エフェクトの座標を更新
	SetPosPlayingEffekseer3DEffect(playingHandle, position.x, position.y, position.z);
}

/*
 *	描画処理
 */
void EffectComponent::Render() {
	if (!isVisible)return;
}
