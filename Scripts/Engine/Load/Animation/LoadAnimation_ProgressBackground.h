/*
 *	@file	LoadAnimation_ProgressBackground.h
 *	@author	Seki
 */

#ifndef _LOAD_ANIMATION_PROGRESS_BACKGROUND_H_
#define _LOAD_ANIMATION_PROGRESS_BACKGROUND_H_

#include "LoadAnimationBase.h"
#include <vector>
#include <string>

 /*
  *	背景切替 + 進捗バー付きロードアニメーション
  */
class LoadAnimation_ProgressBackground : public LoadAnimationBase {
private:
	struct BGImage {
		int spriteHandle;
		float alpha;
	};

	std::vector<BGImage> backgroundList;
	int currentIndex = 0;
	float elapsedTime = 0.0f;
	float switchInterval = 3.0f; // 背景切替間隔
	float fadeTime = 1.0f;       // フェード時間
	bool fadingOut = false;

public:
	LoadAnimation_ProgressBackground() = default;
	~LoadAnimation_ProgressBackground() override { Unload(); }

	// 背景画像読み込み
	void LoadImages(const std::vector<std::string>& paths) {
		for (auto& p : paths) {
			int h = LoadGraph(p.c_str());
			if (h != -1) backgroundList.push_back({ h, 0.0f });
		}
		if (!backgroundList.empty()) backgroundList[0].alpha = 255.0f;
	}

	// 更新処理
	void Update(float unscaledDeltaTime) override;

	// 描画処理
	void Render() override;

	// 解放処理
	void Unload() override;

	// 背景切替間隔設定
	void SetSwitchInterval(float t) { switchInterval = t; }

	// フェード時間設定
	void SetFadeTime(float t) { fadeTime = t; }
};

#endif // !_LOAD_ANIMATION_PROGRESS_BACKGROUND_H_