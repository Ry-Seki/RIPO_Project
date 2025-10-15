/*
 *	@file	LoadAnimation_ChangeBackground.h
 *	@author	Seki
 */

#ifndef _LOAD_ANIMATION_CHANGE_BACKGROUND_H_
#define _LOAD_ANIMATION_CHANGE_BACKGROUND_H_

#include "LoadAnimationBase.h"
#include <vector>
#include <string>

/*
 *	複数枚の背景画像の切り替えるロードアニメーションクラス
 */
class LoadAnimation_ChangeBackground : public LoadAnimationBase {
private:
	struct BGImage {
		int spriteHandle;
		float alpha;
	};

	std::vector<BGImage> backgroundList;	// 背景画像リスト
	int currentIndex = 0;					// 現在のインデクス
	float elapsedTime = 0.0f;				// 経過時間
	float switchInterval = 3.0f;			// 画像切り替え時間(秒)
	float fadeTime = 1.0f;					// フェード時間
	bool fadingOut = false;					// フェードフラグ

public:
	// 画像を読み込む（ファイルパスリスト）
	void LoadImages(const std::vector<std::string>& paths) {
		for (auto& p : paths) {
			int h = LoadGraph(p.c_str());
			if (h != -1) {
				backgroundList.push_back({ h, 0.0f });
			}
		}
		if (!backgroundList.empty())
			backgroundList[0].alpha = 255.0f; // 最初の画像は表示開始
	}
	/*
	 *	更新処理
	 */
	void Update(float unscaledDeltaTime) override;
	/*
	 *	描画処理
	 */
	void Render() override;
};

#endif // !_LOAD_ANIMATION_CHANGE_BACKGROUND_H_