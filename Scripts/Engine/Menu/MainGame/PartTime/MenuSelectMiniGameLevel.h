/*
 *	@file	MenuSelectMiniGameLevel.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_MINI_GAME_LEVEL_H_
#define _MENU_SELECT_MINI_GAME_LEVEL_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"

#include <functional>

 // 前方宣言
class Engine;

/*
 *	@brief	ミニゲーム難易度選択メニュー
 */
class MenuSelectMiniGameLevel : public MenuBase {
private:
	float animTimer = 0.0f;
	int animFrame = 0;

	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;

	std::function<void(GameEnum::MiniGameLevel)> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Dungeon/Result/PlayerDeathMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Dungeon/Result/PlayerDeathMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectMiniGameLevel() override {}

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
	 *	@brief	メニューを再開
	 */
	void Resume() override;

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);

public:
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::MiniGameLevel)> setCallback) {
		Callback = setCallback;
	}

};

#endif // !_MENU_SELECT_MINI_GAME_LEVEL_H_