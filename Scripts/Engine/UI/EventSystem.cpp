/*
 *	@file	EventSystem.cpp
 *	@author	Seki
 */

#include "EventSystem.h"
#include "../Input/InputUtility.h"
#include "Button/UIButtonBase.h"

/*
 *	@brief	初期化処理
 */
void EventSystem::Initialize(int startIndex) {
	if (buttonList.empty()) return;

	if (startIndex < 0 || startIndex >= buttonList.size()) {
		currentIndex = 0;
	} else {
		currentIndex = startIndex;
	}
}
/*
 *	@brief	更新処理
 */
void EventSystem::Update(float unscaledDeltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);
	float vertical = input.axis[static_cast<int>(GameEnum::MenuAction::Vertical)];
	float horizontal = input.axis[static_cast<int>(GameEnum::MenuAction::Horizontal)];

	int nextIndex = currentIndex;

	// 入力処理
#pragma region	入力管理は別クラスがすべき
	if (isInput) {
		elapsedTime += unscaledDeltaTime;
		if (elapsedTime > _DURATION_TIME) {
			isInput = false;
			elapsedTime = 0.0f;
		}
	} else {
		if (horizontal < 0.0f) {
			isInput = true;
			nextIndex = FindNextEnableButton(currentIndex, GameEnum::NavigationDir::Left);
		} else if (horizontal > 0.0f) {
			isInput = true;
			nextIndex = FindNextEnableButton(currentIndex, GameEnum::NavigationDir::Right);
		} else if (vertical > 0.0f) {
			isInput = true;
			nextIndex = FindNextEnableButton(currentIndex, GameEnum::NavigationDir::Up);
		} else if (vertical < 0.0f) {
			isInput = true;
			nextIndex = FindNextEnableButton(currentIndex, GameEnum::NavigationDir::Down);
		}
	}
#pragma endregion
	// 適応
	if (nextIndex != currentIndex) {
		currentIndex = nextIndex;
		ApplySelection();
	}
}
/*
 *	@brief		選択状態の反映
 */
void EventSystem::ApplySelection() {
	currentButton = nullptr;

	for (int i = 0; i < buttonList.size(); ++i) {
		auto& button = buttonList[i];
		if (!button) continue;

		bool isSelect = (i == currentIndex) && button->IsEnable();
		button->SetSelectState(isSelect);

		if (isSelect) currentButton = button;
	}
}
/*
 *	@brief		選択状態のボタンを探す処理
 *	@param[in]	int fromIndex
 *	@param[in]	GameEnum::NavigationDir dir
 *	@return		bool
 */
int EventSystem::FindNextEnableButton(int fromIndex, GameEnum::NavigationDir dir) {
	int current = fromIndex;
	int safety = 0;
	// 無限ループ対策
	while (safety < buttonList.size()) {
		auto itr = navigationMap.find(current);
		if (itr == navigationMap.end()) break;

		const Navigation& navigation = itr->second;

		int next = -1;
		// 方向によって行き先を決定
		switch (dir) {
			case GameEnum::NavigationDir::Left:  next = navigation.left;  break;
			case GameEnum::NavigationDir::Right: next = navigation.right; break;
			case GameEnum::NavigationDir::Up:    next = navigation.up;    break;
			case GameEnum::NavigationDir::Down:  next = navigation.down;  break;
		}
		// 方向が無効な数字の場合、抜ける
		if (next < 0 || next >= buttonList.size()) break;
		// 有効状態ならそれを返す
		if (buttonList[next]->IsEnable()) return next;

		current = next;
		safety++;
	}

	return fromIndex;
}
/*
 *	@brief		移動の道筋データの設定
 *  @param[in]	const JSON& json
 */
void EventSystem::LoadNavigation(const JSON& json) {
	navigationMap.clear();

    // JSON->Navigationへ変換
    for (auto itr = json.begin(); itr != json.end(); ++itr) {
        int index = std::stoi(itr.key());

        Navigation navigation;
        const auto& value = itr.value();

        navigation.left = value.value("Left", -1);
        navigation.right = value.value("Right", -1);
        navigation.up = value.value("Up", -1);
        navigation.down = value.value("Down", -1);

        navigationMap.emplace(index, navigation);
    }
}
/*
 *	@brief		選択状態の更新処理
 *  @param[in]	UIButtonBase* setButton
 */
void EventSystem::UpdateSelectButton(UIButtonBase* setButton) {
	if (!setButton || !setButton->IsEnable()) return;

	for (int i = 0; i < buttonList.size(); ++i) {
		if (buttonList[i] == setButton) {
			currentIndex = i;
			ApplySelection();
			break;
		}
	}
}