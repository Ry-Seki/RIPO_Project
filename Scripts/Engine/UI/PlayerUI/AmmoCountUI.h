/*
 *	@file	AmmoCountUI.h
 *	@author Riku
 */

#ifndef _AMMOCOUNTUI_H_
#define _AMMOCOUNTUI_H_

#include "../../Menu/MenuBase.h"

 // 前方宣言
class Engine;

/*
 *	残弾数UIのクラス
 */
class AmmoCountUI : public MenuBase {
	
public:
	/*
	 *	@brief	デストラクタ
	 */
	~AmmoCountUI() override {};

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override {}
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

};

#endif // !_AMMOCOUNTUI_H_

