/*
 *	@file	MenuSelectTraining.h
 *	@author	Seki
 */

#ifndef _MENU_SELECT_TRAINING_H_
#define _MENU_SELECT_TRAINING_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"

#include <functional>

/*
 *	@brief	トレーニング選択メニュー
 */
class MenuSelectTraining : public MenuBase {
private:
	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;
	Sprite* currentSelect = nullptr;
	std::unordered_map<std::string, UIButtonBase*> buttonMap;
	std::function<void(GameEnum::PlayerStatusType)> Callback = nullptr;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuSelectTraining() override {}

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
	 *	@param[in]	GameEnum::PlayerStatusType type
	 */
	void SelectButtonExecute(GameEnum::PlayerStatusType type);
	/*
	 *	@brief		フェード後->コールバックの実行処理
	 *	@param[in]	GameEnum::PlayerStatusType type
	 */
	void StartFadeEndCallback(GameEnum::PlayerStatusType type);
	/*
	 *	@brief		確認メニューを開く
	 *	@param[in]	GameEnum::PlayerStatusType type
	 */
	void OpenConfirmMenu(GameEnum::PlayerStatusType type);
	/*
	 *	@brief		トレーニングボタンの準備前処理
	 *	@param[in]	const JSON& json
	 */
	void SetupTrainingButtons(const JSON& json);
	/*
	 *	@brief		名前でのボタン検索
	 *	@param[in]	const std::string& buttonName
	 *	@return		UIButtonBase*
	 */
	UIButtonBase* FindButtonByName(const std::string& buttonName);

public:
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void(GameEnum::PlayerStatusType)> setCallback
	 */
	inline void SetCallback(std::function<void(GameEnum::PlayerStatusType)> setCallback) {
		Callback = setCallback;
	}
};

#endif // !_MENU_TRAINING_H_