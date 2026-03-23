/*
 *	ScreenEffectManager.h
 *	@author Riku
 */

#ifndef _SCREENEFFECTMANAGER_H_
#define _SCREENEFFECTMANAGER_H_

#include "../Singleton.h"
#include <functional>

class ScreenEffectManager : public Singleton<ScreenEffectManager> {
	// フレンド宣言
	friend class Singleton<ScreenEffectManager>;
private:
	std::function<void()> renderEffect;	// 描画する画面効果
	float renderMaxCount;				// 描画時間
	float renderCounter;				// 描画時間カウンター

private:
	ScreenEffectManager() = default;
	~ScreenEffectManager() = default;

public:
	/*
	 *	画面効果描画
	 */
	void ScreenEffectRender();

	/*
	 *	ダメージフラッシュ
	 */
	void DamageFlash();

};

#endif // !_SCREENEFFECTMANAGER_H_
