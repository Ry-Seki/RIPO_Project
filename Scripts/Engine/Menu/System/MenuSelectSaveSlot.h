/*
 *	@file	MenuSelectSaveSlot.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_SAVE_SLOT_H_
#define _MENU_SELECT_SAVE_SLOT_H_

#include "../MenuBase.h"
#include "../../UI/Button/UIButtonBase.h"
#include "../../UI/Sprite/Sprite.h"
#include "../../UI/Text/TextBase.h"
#include "../../UI/EventSystem.h"
#include "../../GameEnum.h"
#include "../../GameConst.h"
#include "../../../Data/SaveData.h"

#include <vector>
#include <memory>
#include <cassert>

// 前方宣言
class Engine;

/*
 *	@brief	セーブスロットを選択するメニュー
 */
class MenuSelectSaveSlot : public MenuBase {
private:
	using TextBasePtr = std::shared_ptr<TextBase>;

public:
	/*
	 *	@brief	セーブスロット情報テキスト構造体
	 */
	struct SlotTextSet {
		TextBasePtr elapsedDay;
		TextBasePtr halfDay;
		TextBasePtr playTime;
		TextBasePtr money;
		TextBasePtr treasure;
	};
	/*
	 *	@brief	セーブスロットボタン構造体
	 */
	struct SaveSlotButtonEntry {
		int slotIndex = -1;
		UIButtonBase* button = nullptr;
		SlotTextSet text;
	};
	/*
	 *	@brief	セーブスロットデータ構造体
	 */
	struct SaveSlotData {
		GameProgressData data;
		bool isUsed = false;
	};

private:
	GameEnum::SaveSlotMenuMode saveMode;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;

	Sprite* saveLoadSprite = nullptr;
	std::vector<SaveSlotButtonEntry> slotButtonList;

	std::vector<SaveSlotData> slotDataList;

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
	 *	@brief		セーブ処理
	 *  @param[in]  int slotIndex
	 */
	void ExecuteSave(int slotIndex, Engine& engine);
	/*
	 *	@brief		ロード処理
	 *  @param[in]  int slotIndex
	 */
	void ExecuteLoad(int slotIndex, Engine& engine);

private:
	/*
	 *	@brief		ボタンのコールバック登録
	 *	@param[in]	int slotIndex
	 */
	void SelectButtonExecute(int slotIndex, Engine& engine);
	/*
	 *	@brief		戻る処理
	 */
	void ExecuteBack();
	/*
	 *	@brief		確認メニューを開く
	 *	@param[in]	int slotIndex
	 */
	void OpenConfirmMenu(int slotIndex, Engine& engine);
	/*
	 *	@brief		セーブスロットボタンの準備前処理
	 */
	void InitializeSaveSlotButtons(Engine& engine);
	/*
	 *	@brief		セーブスロットデータの準備前処理
	 */
	void SetupSaveSlotData();
	/*
	 *	@brief	テキストの生成(セーブスロットのテキスト描画用)
	 */
	void CreateSlotText();
	/*
	 *	@brief	テキストの準備前処理
	 */
	void SetupAllSlotText();
	/*
	 *	@brief	スロットボタンの状態更新処理
	 */
	void UpdateSlotButtonState();
	/*
	 *	@brief		セーブスロット情報の描画
	 */
	void RenderSlotInfo();
	/*
	 *	@brief		セーブスロットテキストの更新
	 *	@param[in]	const GameProgressData& data
	 *	@param[in]	SlotTextSet& text
	 */
	void UpdateSlotText(const GameProgressData& data, SlotTextSet& text);
	/*
	 *	@brief		セーブスロット情報の更新
	 *	@param[in]	SaveSlotButtonEntry& entry
	 */
	void UpdateSaveSlot(SaveSlotButtonEntry& entry);
	/*
	 *	@brief		プレイ時間の変換処理
	 *	@param[in]	int playTime
	 *	@return		std::string
	 */
	std::string ChangePlayTimeText(int playTime);

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