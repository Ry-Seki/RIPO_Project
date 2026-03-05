/*
 *	@file	MenuVolumeSettings.h
 *	@author	Seki
 */

#ifndef _MENU_SETTINGS_H_
#define _MENU_SETTINGS_H_

#include "../MenuBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/Text/TextBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;

/*
 *	@brief	設定メニュー
 */
class MenuVolumeSettings : public MenuBase {
private:
	float animTimer = 0.0f;
	int animFrame = 0;

	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	EventSystem eventSystem;

	std::vector<int> volumeTextList;
	static constexpr const float _MOVE_VALUE = 0.1f;
	static constexpr const int _UI_MAX_VOLUME = 10;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/Settings/SettingsResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/Settings/SettingsNavigation.json";

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

private:
	/*
	 *	@brief		ボタンの押された時の処理
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);
	/*
	 *	@brief		セーブデータから音量テキストの設定
	 */
	void SetVolumeText();
	/*
	 *	@brief		音量の変更処理
	 *	@param[in]	int textNum
	 */
	void ChangeVolumeText(int textNum);
};

#endif // !_MENU_SETTINGS_H_