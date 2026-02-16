/*
 *	@file	MenuInGame.h
 *	@author	Seki
 */

#ifndef _MENU_IN_GAME_H_
#define _MENU_IN_GAME_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;

/*
 *	@brief	インゲームメニュー
 */
class MenuInGame : public MenuBase {
private:
	int currentSlot = -1;
	float animTimer = -1;
	int animFrame = -1;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	EventSystem eventSystem;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/InGame/InGameMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/InGame/InGameMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuInGame() override{}

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
	 *	@brief	アニメーション等の更新
	 */
	void AnimUpdate(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close(Engine& engine) override;
	/*
	 *	@brief	メニューを中断
	 */
	void Suspend() override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);

};

#endif // !_MENU_IN_GAME_H_