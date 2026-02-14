/*
 *	@file	MenuMoneyChange.h
 *	@author	Seki
 */

#ifndef _MENU_MONEY_CHANGE_H_
#define _MENU_MONEY_CHANGE_H_

#include "../../MenuBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"

#include <functional>

// 前方宣言
class Engine;

/*
 *	@brief	お金の増減メニュー
 */
class MenuMoneyChange : public MenuBase {
private:
	int currentIndex = -1;
	float animTimer = 0.0f;
	int animFrame = 0;
	int prevMoney = -1;
	int currentMoney = -1;
	int changeMoney = -1;
	int randomMoney = -1;
	float elapsedTime = 0.0;
	bool isAnimationEnd = false;

	EventSystem eventSystem;

	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
	std::vector<std::shared_ptr<Sprite>> spriteList;

	std::function<void()> Callback = nullptr;

	static constexpr float _CHANGE_DURATION = 3.0f;
	static constexpr const char* _MENU_RESOURCES_PATH = "Data/UI/MainGame/Money/ChangeMoneyMenuResources.json";
	static constexpr const char* _NAVIGATION_PATH = "Data/UI/MainGame/Money/ChangeMoneyMenuNavigation.json";

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuMoneyChange() override {}

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
	/*
	 *	@brief		収支の演出
	 *	@return		bool
	 */
	bool ShowChangeMoney(float unscaledDeltaTime);
	/*
	 *	@brief		現在のお金の桁が同じ数字の最小数と最大数を返す
	 *	@param[out] int& minDigitMoney
	 *	@param[out]	int& maxDigitMoney
	 */
	void GetDigitMoney(int& minDigitMoney, int& maxDigitMoney);

public:
	/*
	 *	@brief		前回のお金の設定
	 *	@param[in]	int setValue
	 */
	inline void SetPrevMoney(int setValue) {
		prevMoney = setValue;
	}
	/*
	 *	@brief		現在のお金の取得
	 *	@param[in]	int setValue
	 */
	inline void SetCurrentMoney(int setValue) {
		currentMoney = setValue;
	}
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void()> setCallback) {
		Callback = setCallback;
	}

};

#endif // !_MENU_MONEY_CHANGE_H_