/*
 *	@file	LoadManager.cpp
 *	@author	Seki
 */

#include "LoadManager.h"

/*
 *  @brief  更新処理
 */
void LoadManager::Update(float unscaledDeltaTime) {
    // ロードシステムの更新
    system.Update(unscaledDeltaTime);
    // コールバックの使用
    ActiveCallback();
}
/*
 *  @brief  描画処理
 */
void LoadManager::Render() {
    system.Render();
}
/*
 *  @brief  コールバック登録
 */
void LoadManager::SetOnComplete(const std::function<void()>& callback) {
    if (!callback) return;

    // キューに積む
    onCompleteQueue.push(callback);
}
/*
 *  @brief  コールバックの実行
 */
void LoadManager::ActiveCallback() {
    if (!system.IsComplete()) return;
    // キューに積まれたコールバックの実行（登録順）
    while (!onCompleteQueue.empty()) {
        auto callback = onCompleteQueue.front();
        onCompleteQueue.pop();

        if (callback) callback();
    }
    system.Clear();
}

void LoadManager::Clear() {
    loadRegistry.Clear();
}
