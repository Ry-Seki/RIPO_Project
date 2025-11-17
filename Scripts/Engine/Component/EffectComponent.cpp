/*
 *	@file	EffectComponent.cpp
 *  @author oorui
 */

#include "EffectComponent.h"
#include <EffekseerForDxLib.h>
 /*
  * コンストラクタ
  */
EffectComponent::EffectComponent(int& _resourceHandle)
	:resourceHandle(_resourceHandle)
	, playingHandle(-1)
	, isLoop(false) {
}

/*
 *	エフェクト再生
 *  @tips	ループの場合でも使用可能
 */
void EffectComponent::EffectRenderer() {
	if (!isVisible)return;

	// エフェクト再生
	if (playingHandle == -1)
		playingHandle = PlayEffekseer3DEffect(resourceHandle);

	// 再生が終わったら
	if (!IsEffekseer3DEffectPlaying(playingHandle))
		isVisible = false;

	// エフェクトの座標を更新
	SetPosPlayingEffekseer3DEffect(playingHandle, position.x, position.y, position.z);
}

/*
 *	描画処理
 */
void EffectComponent::Render() {
	if (!isVisible)return;
}
