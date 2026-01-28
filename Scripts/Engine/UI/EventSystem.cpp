/*
 *	@file	EventSystem.cpp
 */

#include "EventSystem.h"
#include "../Input/InputUtility.h"

/*
 *	@brief	初期化処理
 */
void EventSystem::Initialize() {

}
/*
 *	@brief	更新処理
 */
void EventSystem::Update(float deltaTime) {
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
