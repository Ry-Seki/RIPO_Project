/*
 *	@file	MenuSystem.h
 *	@author	Seki
 */

#ifndef _MENU_SYSTEM_H_
#define _MENU_SYSTEM_H_

#include "../MenuBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"

// 前方宣言
class Engine;

/*
 *	@brief	システムメニュー
 */
class MenuSystem : public MenuBase {
private:
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;

	std::unordered_map<std::string, UIButtonBase*> buttonMap;

	EventSystem eventSystem;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSystem() override {};

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
	 *	@param[in]	GameEnum::SystemMenuType type
	 */
	void SelectButtonExecute(GameEnum::SystemMenuType type);
	/*
	 *	@brief		システムボタンの準備前処理
	 *	@param[in]	const JSON& json
	 */
	void SetupSystemButtons(const JSON& json);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

};

#endif // !_MENU_SYSTEM_H_
