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
	float fadeTime = 1.0f;					// 2秒かけてフェード
	float switchInterval = 0.2f;			// 4秒ごとに切り替え
	bool fadingOut = false;					// フェードフラグ
	int frameCounter = 0;					// フレームカウンタ
	int switchFrame = 3;					// ← 何フレームごとに切り替えるか（例：3）

public:
	~LoadAnimation_ChangeBackground() override = default;

public:
	/*
	 *	画像の設定
	 *  @param[in]	const std::vector<int>& handleList
	 */
	void SetImages(const std::vector<int>& handleList) {
		for (int i = 0, max = handleList.size(); i < max; i++) {
			int handle = handleList[i];
			if (handle == -1) continue;

			backgroundList.push_back({ handle, 0.0f });

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

	// 画像解放
	void Unload() override;
};

#endif // !_LOAD_ANIMATION_CHANGE_BACKGROUND_H_