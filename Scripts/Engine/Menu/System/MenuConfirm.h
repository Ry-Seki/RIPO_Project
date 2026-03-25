/*
 *	@file	MenuConfirm.h
 *	@author	Seki
 */

#ifndef _MENU_DIALOG_H_
#define _MENU_DIALOG_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

#include <functional>

// 前方宣言
class Engine;

/*
 *	@brief	確認メニュー
 */
class MenuConfirm : public MenuBase {
private:
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;

	std::function<void(GameEnum::ConfirmResult)> Callback = nullptr;
	
public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuConfirm() override {}

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
	 *	@param[in]	GameEnum::ConfirmResult result
	 */
	void SelectButtonExecute(GameEnum::ConfirmResult result);
	/*
	 *	@brief		確認ボタンの準備前処理
	 *	@param[in]	const JSON& json
	 */
	void SetupConfirmButtons(const JSON& json);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

public:
	/*
	 *	@brief	
	 */
	inline bool IsBlocking() const override { return true; }
	/*
	 *	@brief		コールバックの登録
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::ConfirmResult)> setCallback) {
		if (!setCallback) return;

		Callback = setCallback;
	}

};

#endif // !_MENU_DIALOG_H_