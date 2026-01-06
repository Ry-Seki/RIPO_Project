
/*
 *	@file	AnimatorComponent.cpp
 *  @author oorui
 */
#include "AnimatorComponent.h"


AnimatorComponent::AnimatorComponent()
	: animModelHandle(-1)
	, currentAnimation(-1)
	, animIndex(0)
	, attachIndex(-1)
	, isPlaying(false)
{
}

/*
 *	@function	Update
 *	@brief		更新処理
 */
void AnimatorComponent::Update(float deltaTime) {
	// 無効なアニメーションだった場合は処理しない
	if (currentAnimation == -1)return;

	// 現在のアニメーションを取得
	AnimatorClip* pCurrentAnim = GetAnimation(currentAnimation);
	if (pCurrentAnim == nullptr)return;

	// アニメーションを進める
	pCurrentAnim->playAnimTime += pCurrentAnim->playAnimSpeed;

	// 再生時間を超えたら
	if (pCurrentAnim->playAnimTime > pCurrentAnim->exitAnimTime) {
		// 再生中フラグをfalseにする
		isPlaying = false;
		// 再生時間をリセットする
		pCurrentAnim->playAnimTime = 0.0f;

		// 再生していたアニメーションがループ再生するかどうか
		if (pCurrentAnim->isAnimLoop) {
			isPlaying = true;
		}
		else {
			// 終了後のアニメーションがループ再生するかどうか
			Play(pCurrentAnim->animTransitionNum);
		}

	}

	// アニメーションを再生
	MV1SetAttachAnimTime(animModelHandle, attachIndex, pCurrentAnim->playAnimTime);

}


/*
 *	アニメーションの名前で番号を読み込む
 *  @param		isLoop		// ループするかどうか
 *  @param		transition	// 終了後の番号
 */
void AnimatorComponent::LoadIndex(bool isLoop, int transition) {
	if (animModelHandle == -1) {
		return;
	}

	// モデル内のアニメーション数を取得
	int animCount = MV1GetAnimNum(animModelHandle);
	if (animCount <= 0) {
		return;
	}

	pAnimations.reserve(animCount);

	// モデルに含まれる全アニメーションを登録
	for (int i = 0; i < animCount; ++i) {
		auto* clip = new AnimatorClip(i, isLoop, transition);
		clip->exitAnimTime = MV1GetAnimTotalTime(animModelHandle, i);
		pAnimations.push_back(clip);
	}

}

/*
 *	アニメーションの再生
 *  @param	index
 *  @param  spped = 1.0f;
 */
void AnimatorComponent::Play(int index, float speed) {
	// 現在再生中のアニメーションの場合は処理しない
	if (index == animIndex)return;
	// 同じアニメーションなら再生しない
	if (currentAnimation == index) return;
	if (attachIndex != -1)
		MV1DetachAnim(animModelHandle, attachIndex);

	// 現在のアニメーションに設定する
	currentAnimation = index;
	// アニメーションハンドルに登録
	auto* anim = pAnimations[index];
	// アニメーションの再生時間を初期化
	anim->playAnimTime = 0.0f;
	// アニメーションスピードを指定されたスピードに設定
	anim->playAnimSpeed = speed;

	// モデル内のアニメーション番号を直接指定してアタッチ
	attachIndex = MV1AttachAnim(animModelHandle, anim->animationHandle);
	// 終了時間を初期化
	anim->exitAnimTime = MV1GetAttachAnimTotalTime(animModelHandle, attachIndex);

	// 再生中に設定
	isPlaying = true;
}
