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
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;

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
	 *	@param[in]	GameEnum::InGameMenuType type
	 */
	void SelectButtonExecute(GameEnum::InGameMenuType type, Engine& engine);
	/*
	 *	@brief		メニューボタンの初期化処理
	 *	@param[in]	const JSON& json
	 */
	void InitializeMenuButtons(const JSON& json, Engine& engine);
	/*
	 *	@brief		フェード後->コールバックの実行処理
	 *	@param[in]	GameEnum::InGameMenuType type
	 */
	void StartFadeEndCallback(GameEnum::InGameMenuType type, Engine& engine);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

public:
	/*
	 *	@brief	ロード画面を開く
	 */
	void ExecuteLoadMenu(Engine& engine);
	/*
	 *	@brief	セーブ画面を開く
	 */
	void ExecuteSaveMenu(Engine& engine);
	/*
	 *	@brief	設定画面を開く
	 */
	void ExecuteSettingsMenu(Engine& engine);
	/*
	 *	@brief	タイトル画面に戻る
	 */
	void ExecuteReturnTitle(Engine& engine);
	/*
	 *	@brief	前の画面に戻る
	 */
	void ExecuteReturnPrevMenu(Engine& engine);
};

#endif // !_MENU_IN_GAME_H_