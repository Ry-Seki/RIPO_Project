/*
 *	@file	EventSystem.h
 *	@author	Seki
 */

#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include "../GameEnum.h"
#include "../JSON.h"

#include <vector>
#include <unordered_map>

// 前方宣言
class UIButtonBase;
class ActionMapBase;
struct ActionState;

/*
 *	@brief	移動の道筋構造体
 */
struct Navigation {
	int left = -1;
	int right = -1;
	int up = -1;
	int down = -1;
};

/*
 *	@brief	入力によって選択を動かすクラス(Menu派生クラス内、ボタンクラス限定)
 *  @tips	一旦、現時点では選択入力をここにしているが、
 *			いずれかに入力管理部分を分離して選択状態の管理のみにする予定
 *			また、ゲームの規模次第ではボタンだけではなくUnityみたく、Seletableクラスを作る必要がある。
 */
class EventSystem {
private:
	int currentIndex = -1;
	float elapsedTime = 0.0f;
	bool isInput = false;
	std::vector<UIButtonBase*> buttonList;
	UIButtonBase* currentButton = nullptr;
	std::unordered_map<int, Navigation> navigationMap;

	const float _DURATION_TIME = 0.2f;

public:
	/*
	 *	@brief	コンストラクタ
	 */
	EventSystem() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~EventSystem() = default;

public:
	/*
	 *	@brief		初期化処理
	 *  @param[in]	int startIndex
	 */
	void Initialize(int startIndex);
	/*
	 *	@brief	更新処理
	 */
	void Update(float unscaledDeltaTime);

private:
	/*
	 *	@brief		選択状態のボタンを探す処理
	 *	@param[in]	int fromIndex
	 *	@param[in]	GameEnum::NavigationDir dir
	 *	@return		bool
	 */
	int FindNextEnableButton(int index, GameEnum::NavigationDir dir);

public:
	/*
	 *	@brief		選択状態の反映
	 */
	void ApplySelection();
	/*
	 *	@brief		移動の道筋データの設定
	 *  @param[in]	const JSON& json
	 */
	void LoadNavigation(const JSON& json);
	/*
	 *	@brief		選択状態の更新処理
	 *  @param[in]	UIButtonBase* setButton
	 */
	void UpdateSelectButton(UIButtonBase* setButton);

public:
	/*
	 *	@brief		現在選択されているボタンを取得
	 *	@return		UIButtonBase*
	 */
	inline UIButtonBase* GetCurrentSelectButton() const{ return currentButton; }
	/*
	 *	@brief		ボタンの登録
	 *  @param[in]	UIButtonBase* setButton
	 */
	inline void RegisterButton(UIButtonBase* setButton) {
		if (!setButton) return;

		buttonList.push_back(setButton);
	}
};

#endif // !_EVENT_SYSTEM_H_