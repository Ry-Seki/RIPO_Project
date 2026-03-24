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
	int damageFlashGraphHandle;	

	const float DAMAGE_FLASH_COUNT;
	const float DAMAGE_FLASH_PARAM;
	static constexpr const char* DAMAGE_FLASH_PATH = "Res/ScreenEffect/DamageFlash.png";

private:
	ScreenEffectManager();
	~ScreenEffectManager() = default;

public:
	/*
	 *	初期化
	 */
	void Initialize();

	/*
	 *	画面効果描画
	 */
	void ScreenEffectRender();

	/*
	 *	ダメージフラッシュ
	 */
	void DamageFlash();

private:
	/*
	 *	グラフハンドルセット
	 */
	inline void SetGraphHandle(int setGraph) { damageFlashGraphHandle = setGraph; }
};

#endif // !_SCREENEFFECTMANAGER_H_
