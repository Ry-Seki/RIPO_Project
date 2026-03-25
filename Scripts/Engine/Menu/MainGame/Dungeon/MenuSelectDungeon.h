/*
 *	@file	MenuSelectDungeon.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_DUNGEON_H_
#define _MENU_SELECT_DUNGEON_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/Text/TextBase.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"
#include "../../../../Data/Dungeon/DungeonData.h"

#include <unordered_map>
#include <functional>

// 前方宣言

/*
 *	@brief	ダンジョン選択メニュー
 */
class MenuSelectDungeon : public MenuBase {	
private:
	/*
	 *	@brief	ダンジョンメニュー構造体
	 */
	struct DungeonMenuEntry {
		DungeonInfoData info;               // ダンジョンデータ
		TextBase* level = nullptr;			// ダンジョンのレベル
		TextBase* strength = nullptr;		// 最小-最大Strength
		TextBase* treasureCount = nullptr;	// お宝情報テキスト
		TextBase* eventInfo = nullptr;		// イベント情報
	};

private:
	int prevIndex = -1;
	std::vector<DungeonInfoData> dungeonInfoList;
	EventSystem eventSystem;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	Sprite* dungeonSprite = nullptr;

	std::vector<DungeonMenuEntry> dungeonMenuList;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;
	std::unordered_map<std::string, Sprite*> spriteMap;
	std::unordered_map<GameEnum::DungeonType, Sprite*> eventSpriteMap;

	std::function<void(int)> Callback = nullptr;

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
	 *	@param[in]	int dungeonID
	 */
	void SelectButtonExecute(int dungeonID);
	/*
	 *	@brief		テキストの生成
	 */
	void CreateDungeonInfoData();
	/*
	 *	@brief		テキストの準備前処理
	 */
	void SetupDungeonInfo();
	/*
	 *	@brief		ダンジョン情報の描画
	 */
	void RenderDungeonInfo();
	/*
	 *	@brief		フェード後->コールバックの実行処理
	 *	@param[in]	int dungeonID
	 */
	void StartFadeEndCallback(int dungeonID);
	/*
	 *	@brief		確認メニューを開く
	 *	@param[in]	int dungeonID
	 */
	void OpenConfirmMenu(int dungeonID);
	/*
	 *	@brief		イベント情報テキストを整理
	 *	@param[in]	DungeonMenuEntry& entry
	 */
	void SortDungeonEventText(DungeonMenuEntry& entry);
	/*
	 *	@brief		イベント画像の準備前処理
	 */
	void SetupEventSprite();
	/*
	 *	@brief		ダンジョンボタンの初期化処理
	 *	@param[in]	const JSON& json
	 */
	void InitializeDungeonButtons(const JSON& json);
	/*
	 *	@brief		イベント画像の初期化処理
	 *	@param[in]	const JSON& json
	 */
	void InitializeEventSprites(const JSON& json);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);
	/*
	 *	@brief		名前での画像検索
	 *	@param[in]	const std::string& spriteName
	 *	@return		Sprite*
	 */
	Sprite* FindSpriteByName(const std::string& spriteName);
	/*
	 *	@brief		ダンジョンの種類からダンジョンのメニュー構造体の取得
	 *	@param[in]	GameEnum::DungeonType type
	 *	@return		DungeonInfoData&
	 */
	DungeonInfoData& GetDungeonInfoData(GameEnum::DungeonType type);

public:
	/*
	 *	@brief		ダンジョン情報データの設定
	 *	@param[in]	const std::vector<DungeonInfoData>& setData
	 */
	inline void SetInfoData(const std::vector<DungeonInfoData>& setData) {
		dungeonInfoList = setData;
	}
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(int)> setCallback
	 */
	inline void SetCallback(std::function<void(int)> setCallback) {
		Callback = setCallback;
	}
};

#endif // !_MENU_SELECT_DUNGEON_H_