/*
 *	@file	MenuSelectMiniGameLevel.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_MINI_GAME_LEVEL_H_
#define _MENU_SELECT_MINI_GAME_LEVEL_H_

#include "../../MenuBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/Text/TextBase.h"
#include "../../../UI/Button/UIButtonBase.h"
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
	std::vector<int> rewardList;

	EventSystem eventSystem;

	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;
	std::unordered_map<std::string, TextBase*> textMap;
	std::unordered_map<GameEnum::MiniGameLevel, TextBase*> rewardMap;

	std::function<void(GameEnum::MiniGameLevel)> Callback = nullptr;

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
	 *	@param[in]	GameEnum::MiniGameLevel level
	 */
	void SelectButtonExecute(GameEnum::MiniGameLevel level);
	/*
	 *	@brief		フェード後->コールバックの実行処理
	 *	@param[in]	GameEnum::MiniGameLevel level
	 */
	void StartFadeEndCallback(GameEnum::MiniGameLevel level);
	/*
	 *	@brief		確認メニューを開く
	 *	@param[in]	GameEnum::MiniGameLevel level
	 */
	void OpenConfirmMenu(GameEnum::MiniGameLevel level);
	/*
	 *	@brief		難易度ボタンの初期化処理
	 *	@param[in]	const JSON& json
	 */
	void InitializeLevelButtons(const JSON& json);
	/*
	 *	@brief		難易度別報酬テキストの初期化処理
	 *	@param[in]	const JSON& json
	 */
	void InitializeRewardTexts(const JSON& json);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);
	/*
	 *	@brief		名前でのテキスト検索
	 *	@param[in]	const std::string& textName
	 *	@return		TextBase*
	 */
	TextBase* FindTextByName(const std::string& textName);

public:
	/*
	 *	@brief		難易度別報酬テキストの設定
	 */
	void SetRewardTexts();

public:
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::MiniGameLevel)> setCallback) {
		Callback = setCallback;
	}
	/*
	 *	@brief		報酬リストの設定
	 *	@param[in]	const std::vector<int>& setList
	 */
	inline void SetLevelRewardList(const std::vector<int>& setList) {
		rewardList = setList;
	}
};

#endif // !_MENU_SELECT_MINI_GAME_LEVEL_H_