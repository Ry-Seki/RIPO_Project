/*
 *	@file	MenuSelectDungeon.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_DUNGEON_H_
#define _MENU_SELECT_DUNGEON_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"

#include <functional>

// 前方宣言
struct DungeonInfoData;

/*
 *	@brief	ダンジョン選択メニュー
 */
class MenuSelectDungeon : public MenuBase {
private:
	int currentIndex = -1;
	float animTimer = 0.0f;
	int animFrame = 0;

	std::vector<DungeonInfoData> dungeonInfoList;
	std::vector<std::string> dungeonString;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	Sprite* dungeonSprite = nullptr;

	std::function<void(int)> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Dungeon/SelectDungeon/SelectDungeonMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame//Dungeon/SelectDungeon/SelectDungeonMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectDungeon() override {}

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
	 *	@brief		ダンジョン情報の描画
	 */
	void RenderDungeonInfo();

public:
	/*
	 *	@brief		半日フラグの設定
	 *	@param[in]	const std::vector<DungeonInfoData>& setData
	 */
	inline void SetInfoData(const std::vector<DungeonInfoData>& setData) { dungeonInfoList = setData; }
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(int)> setCallback
	 */
	inline void SetCallback(std::function<void(int)> setCallback) {
		Callback = setCallback;
	}
};

#endif // !_MENU_SELECT_DUNGEON_H_