/*
 *	@file	PlayerUI.h
 *	@author Riku
 */

#ifndef _PLAYERUI_H_
#define _PLAYERUI_H_

#include "../../Menu/MenuBase.h"

 // 前方宣言
class Engine;

/*
 *	プレイヤーUIのクラス
 */
class PlayerUI : public MenuBase{
	int reticleGraphHandle = -1;

	static constexpr const char* RETICLE_PATH = "Res/PlayerUI/reticle.png";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~PlayerUI() override {};

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
	 *	レティクルのグラフハンドルのセット
	 */
	void SetReticleGraphHandle(int setHandle);
};

#endif // !_PLAYERUI_H_

