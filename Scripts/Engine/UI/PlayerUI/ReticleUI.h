/*
 *	@file	ReticleUI.h
 *	@author Riku
 */

#ifndef _RETICLEUI_H_
#define _RETICLEUI_H_

#include "../../Menu/MenuBase.h"

 // 前方宣言
class Engine;

/*
 *	レティクルUIのクラス
 */
class ReticleUI : public MenuBase{
	int reticleGraphHandle = -1;

	static constexpr const char* RETICLE_PATH = "Res/PlayerUI/reticle.png";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~ReticleUI() override {};

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime) override {}
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;

public:
	/*
	 *	グラフハンドルのセット
	 */
	void SetGraphHandle(int setHandle);
};

#endif // !_RETICLEUI_H_

