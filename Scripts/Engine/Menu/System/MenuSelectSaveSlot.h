/*
 *	@file	MenuSelectSaveSlot.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_SAVE_SLOT_H_
#define _MENU_SELECT_SAVE_SLOT_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"
#include "../../../Data/SaveData.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;

/*
 *	@brief	セーブスロットを選択するメニュー
 */
class MenuSelectSaveSlot : public MenuBase {
private:
	int currentSlot = -1;
	float animTimer = 0.0f;
	int animFrame = 0;
	GameEnum::SaveSlotMenuMode saveMode;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;

	Sprite* saveLoadSprite = nullptr;

	std::vector<GameProgressData> gameDataList;
	std::vector<bool> isUsedList;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/System/SaveLoad/SaveSlotResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/System/SaveLoad/SaveSlotNavigation.json";

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize (Engine& engine) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open () override;
	/*
	 *	@brief	更新処理
	 */
	void Update (Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	アニメーション等の更新
	 */
	void AnimUpdate(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render () override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close (Engine& engine) override;
	/*
	 *	@brief	メニューを再開
	 */
	void Resume() override;

public:
	/*
	 *	@brief		ボタンのコールバック登録
	 *	@param[in]	int slotIndex
	 */
	void SelectButtonExecute(Engine& engine, int slotIndex);
	/*
	 *	@brief		セーブスロット情報の描画
	 */
	void RenderSlotInfo();

public:
	/*
	 *	@brief		セーブモードの設定
	 *	@param[in]	GameEnum::SaveSlotMenuMode setSaveMode
	 */
	inline void SetSaveMode(GameEnum::SaveSlotMenuMode setSaveMode) {
		saveMode = setSaveMode;
	}
};

#endif // !_MENU_SELECT_SAVE_SLOT_H_