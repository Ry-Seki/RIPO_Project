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
	int animFrame = 0;
	float animTimer = 0.0f;
	int elapsedDay = -1;
	bool isHalf = false;

	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;

	std::function<void(GameEnum::PlayerStatusType)> Callback = nullptr;

	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Training/SelectTraining/SelectTrainingMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Training/SelectTraining/SelectTrainingMenuNavigation.json";

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
	 *	@param[in]	int buttonIndex
	 */
	void SelectButtonExecute(Engine& engine, int buttonIndex);

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