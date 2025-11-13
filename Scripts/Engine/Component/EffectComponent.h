/*
 *	@file	EffectComponent.h
 *  @author oorui
 */

#ifndef _EFFECT_COMPONENT_H_
#define _EFFECT_COMPONENT_H_
#include"Component.h"

 /*
  *	エフェクトオブジェクト
  * エフェクトの再生などを行う用
  */
class EffectComponent : public Component {
private:
	int resourceHandle;		// エフェクト本体のハンドル
	int playingHandle;		// 再生中のエフェクトハンドル

public:
	/*
	 *	コンストラクタ
	 *  @param		resourcehandle	再生するエフェクトの本体
	 */
	EffectComponent(int& _resourceHandle);
	~EffectComponent() = default;

public:

	/*
	 *	エフェクト再生
	 */
	void EffectRenderer();
};

#endif // !_EFFECT_COMPONENT_H_
