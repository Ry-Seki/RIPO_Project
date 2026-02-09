/*
 *	@file	PlayerUI.h
 *	@author Riku
 */

#ifndef _PLAYERUI_H_
#define _PLAYERUI_H_

#include "../../Menu/MenuBase.h"
#include "PlayerUIBase.h"
#include <vector>

 // 前方宣言
class Engine;

/*
 *	プレイヤーUIのクラス
 */
class PlayerUI : public MenuBase{
private:
	std::vector<PlayerUIBasePtr> UIs;

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
	void Update(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
};

#endif // !_PLAYERUI_H_

