/*
 *	@file	EventSystem.cpp
 */

#include "EventSystem.h"
#include "../Input/InputUtility.h"
#include "Button/UIButton.h"

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
	ApplySelection();
}
/*
 *	@brief	更新処理
 */
void EventSystem::Update(float deltaTime) {
	if (currentIndex < 0) return;

	int nextIndex = currentIndex;
	auto action = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);
	float vertical = action.axis[static_cast<int>(GameEnum::MenuAction::Vertical)];
	float horizontal = action.axis[static_cast<int>(GameEnum::MenuAction::Horizontal)];

	auto itr = navigationMap.find(currentIndex);
	if (itr == navigationMap.end()) return; 

	const Navigation& navigation = itr->second;
	// 入力取得
	if (horizontal < 0.0f) {
		nextIndex = navigation.left;
	} else if (horizontal > 0.0f) {
		nextIndex = navigation.right;
	} else if (vertical > 0.0f) {
		nextIndex = navigation.up;
	} else if (vertical < 0.0f) {
		nextIndex = navigation.down;
	}

	// 移動があった場合のみ反映
	if (nextIndex != currentIndex &&
		nextIndex >= 0 &&
		nextIndex < static_cast<int>(buttonList.size())) {
		currentIndex = nextIndex;
		ApplySelection();
	}

	// 決定
	if (action.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)]) {
		auto& currentButton = buttonList[currentIndex];
		if (!currentButton) return;
		
		currentButton->Execute();
	}
}
/*
 *	@brief	選択状態の反映
 */
void EventSystem::ApplySelection() {
    for (int i = 0, max = buttonList.size(); i < max; ++i) {
        if (!buttonList[i]) continue;

        //buttonList[i]->SetIsSelected(static_cast<int>(i) == currentIndex);
    }
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
