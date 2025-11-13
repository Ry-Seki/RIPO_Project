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
	, playingHandle(-1) {
}

/*
 *	エフェクト再生
 */
void EffectComponent::EffectRenderer() {
	if (!resourceHandle)return;

	// エフェクト再生
	if (playingHandle == -1)
		playingHandle = PlayEffekseer3DEffect(resourceHandle);

	// 再生が終わったら
	if (!IsEffekseer3DEffectPlaying(playingHandle))
		return;
}
