/*
 *	@file	ReticleUI.h
 *	@author Riku
 */
#ifndef _RETICLEUI_H_
#define _RETICLEUI_H_

/*
 *	レティクルUIのクラス
 */
class ReticleUI {
	int reticleGraphHandle = -1;

	static constexpr const char* RETICLE_PATH = "Res/PlayerUI/reticle.png";

public:
	ReticleUI();
	~ReticleUI() = default;

public:
	/*
	 * 初期化
	 */
	void Initialize();

	/*
	 *	描画
	 */
	void Render();

	/*
	 *	グラフハンドルのセット
	 */
	void SetGraphHandle(int setHandle);
};

#endif // !_RETICLEUI_H_

