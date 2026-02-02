/*
 *	@file	MenuTitle.h
 *	@author	Seki
 */

#ifndef _MENU_TITLE_H_
#define _MENU_TITLE_H_

#include "../MenuBase.h"

// 前方宣言
class Engine;

/*
 *	@brief	タイトルメニュー
 */
class MenuTitle : public MenuBase{
private:
	int titleGraphHandle = -1;

	static constexpr const char* _TITLE_LOGO_PATH = "Res/Menu/Title/Trealine_NewTitle.png";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuTitle() override {};

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close(Engine& engine) override;

public:
	/*
	 *	@brief		ロード済みデータのセット
	 *  @prarm[in]	int setHandle
	 */
	void SetupData(int setHandle);
};

#endif // !_MENU_TITLE_H_