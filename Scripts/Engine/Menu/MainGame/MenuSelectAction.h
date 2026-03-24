/*
 *	@file	MenuSelectAction.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_ACTION_H_
#define _MENU_SELECT_ACTION_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"
#include "../../UI/Text/TextBase.h"

#include <functional>

/*
 *	@brief	アクション選択メニュー
 */
class MenuSelectAction : public MenuBase {
private:
	int elapsedDay = -1;
	bool isHalf = false;

	EventSystem eventSystem;

	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	Sprite* elapsedDaySprite = nullptr;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;
	
	std::function<void(GameEnum::ActionType)> Callback = nullptr;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectAction() override {}

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
	 *	@brief		テキストの生成
	 */
	void CreateElapsedDayText();
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	GameEnum::ActionType type
	 */
	void SelectButtonExecute(GameEnum::ActionType type);
	/*
	 *	@brief		アクションボタンの初期化処理
	 *	@param[in]	const JSON& json
	 */
	void InitializeActionButtons(const JSON& json);
	/*
	 *	@brief		フェード後->コールバックの実行処理
	 *	@param[in]	GameEnum::ActionType type
	 */
	void StartFadeEndCallback(GameEnum::ActionType type);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

public:
	/*
	 *	@brief		経過日数の設定
	 *	@param[in]	int setValue
	 */
	inline void SetElapsedDay(int setValue) { elapsedDay = setValue; }
	/*
	 *	@brief		半日フラグの設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsHalf(bool setFlag) { isHalf = setFlag; }
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(GameEnum::ActionType)> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::ActionType)> setCallback) {
		Callback = setCallback;
	}
};

#endif // !_MENU_SELECT_ACTION_H_