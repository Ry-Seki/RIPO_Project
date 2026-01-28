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
class UIButton;

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
 *	@brief	入力によって選択を動かすクラス
 */
class EventSystem {
private:
	int currentIndex = -1;
	std::vector<UIButton*> buttonList;
	std::unordered_map<int, Navigation> navigationMap;

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
	 *	@brief	初期化処理
	 */
	void Initialize();
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime);

public:
	/*
	 *	@brief		移動の道筋データの設定
	 *  @param[in]	const JSON& json
	 */
	void LoadNavigation(const JSON& json);

public:
	/*
	 *	@brief		ボタンの登録
	 *  @param[in]	UIButton* setButton
	 */
	inline void RegisterButton(UIButton* setButton) {
		if (!setButton) return;

		buttonList.push_back(setButton);
	}
};

#endif // !_EVENT_SYSTEM_H_