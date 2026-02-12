/*
 *	@file	MenuTitle.h
 *	@author	Seki
 */

#ifndef _MENU_TITLE_H_
#define _MENU_TITLE_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

// 前方宣言
class Engine;

/*
 *	@brief	タイトルメニュー
 */
class MenuTitle : public MenuBase{
	float animTimer = 0.0f;
	int animFrame = 0;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	Sprite* startGameSprite = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/Title/StartGame/TitleResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/Title/StartGame/TitleNavigation.json";

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

public:
	/*
	 *	@brief		ロード済みデータのセット
	 *  @prarm[in]	int setHandle
	 */
	void SetupData(int setHandle);
private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);

};

#endif // !_MENU_TITLE_H_