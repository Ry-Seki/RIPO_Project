/*
 *	@file	MenuMiniGame.h
 *	@author	Seki
 */

#ifndef _MENU_MINI_GAME_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"

#include <functional>

 // 前方宣言
class Engine;
class MiniGameBase;

/*
 *	@brief	ミニゲームメニュー
 */
class MenuMiniGame : public MenuBase {
public:
	using MiniGamePtr = std::shared_ptr<MiniGameBase>;

private:
	std::vector<MiniGamePtr> miniGameList;
	MiniGamePtr miniGame = nullptr;
	GameEnum::MiniGameLevel level = GameEnum::MiniGameLevel::Invalid;

	float animTimer = 0.0f;
	int animFrame = 0;

	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;

	std::function<void(GameEnum::MiniGameLevel)> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/PartTime/MiniGame/MiniGameMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/PartTime/MiniGame/MiniGameMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuMiniGame() override {}

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
	/*
	 *	@brief		リセット処理
	 */
	void Reset();

public:
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(GameEnum::MiniGameLevel)> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::MiniGameLevel)> setCallback) {
		Callback = setCallback;
	}
	/*
	 *	@brief		ミニゲーム難易度の設定
	 *	@param[in]	GameEnum::MiniGameLevel setLevel
	 */
	inline void SetMiniGameLevel(GameEnum::MiniGameLevel setLevel) {
		level = setLevel;
	}
};
#endif // !_MENU_MINI_GAME_H_