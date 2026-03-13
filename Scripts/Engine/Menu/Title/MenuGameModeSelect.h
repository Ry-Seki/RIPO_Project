/*
 *	@file	MenuGameModeSelect.h
 *	@author	Seki
 */

#ifndef _MENU_GAME_MODE_SELECT_H_
#define _MENU_GAME_MODE_SELECT_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

#include <unordered_map>

// 前方宣言
class Engine;

/*
 *	@brief	ゲームモードを選択するメニュー
 */
class MenuGameModeSelect : public MenuBase {
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuGameModeSelect() override {};

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
	 *	@param[in]	GameEnum::GameMode mode
	 */
	void SelectButtonExecute(GameEnum::GameMode mode, Engine& engine);
	/*
	 *	@brief		ゲームモードボタンの準備前処理
	 *	@param[in]	const JSON& json
	 */
	void SetupGameModeButtons(const JSON& json, Engine& engine);
	/*
	 *	@brief		ゲーム終了処理
	 */
	void CheckEndGame(Engine& engine);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);
};

#endif // !_MENU_GAME_MODE_SELECT_H_