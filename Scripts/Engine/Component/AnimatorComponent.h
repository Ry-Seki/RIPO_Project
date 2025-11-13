/*
 *	@file	AnimatorComponent.h
 *  @author oorui
 */

#ifndef _ANIMATORCOMPONENT_H_
#define _ANIMATORCOMPONENT_H_

#include "component.h"
#include "../Engine.h"

 /*
  *	アニメーションクリップ構造体
  */
struct AnimatorClip {
	int animationHandle;		// アニメーション番号

	float playAnimTime;		// アニメーション再生時間
	float exitAnimTime;		// アニメーションの終了時間
	float playAnimSpeed;	// アニメーションの再生速度

	bool isAnimLoop;		// アニメーションがループするかどうか
	int animTransitionNum;	// アニメーションが終了した後に再生する番号


	/*
	 *	コンストラクタ
	 *	@param[out]	int& _animNum	    　　アニメーションのハンドル
	 *  @param[in]	bool _isLoop = false	ループ再生するかどうか
	 *	@param[in]	int _transition			終了後の番号
	 */
	AnimatorClip(int _animNum, bool _isLoop = false, int _transition = 0)
		: animationHandle(_animNum)
		, playAnimTime(0.0f)
		, exitAnimTime(0.0f)
		, playAnimSpeed(1.0f)
		, isAnimLoop(_isLoop)
		, animTransitionNum(_transition) {
	}

	/*
	 *	デストラクタ
	 */
	~AnimatorClip() {
		MV1DeleteModel(animationHandle);
	}

	/*
	 *	アニメーションハンドルを取得
	 */
	void SetAnimHandle(int _setValue) { animationHandle = _setValue; }
};

class AnimatorComponent : public Component {
private:
	int animModelHandle;					// アニメーションのモデルハンドル
	std::vector<AnimatorClip*> pAnimations;	// アニメーションのハンドル
	int currentAnimation;					// 現在のアニメーション
	int animIndex;							// アニメーション番号
	int attachIndex;						// アタッチ番号
	bool isPlaying;							// 再生中かどうか

public:
	AnimatorComponent() = default;
	virtual ~AnimatorComponent() = default;

public:
	/*
	 *	@function	Update
	 *	@brief		更新処理
	 */
	virtual void Update(float deltaTime) override;

	/*
	 *	アニメーションの名前で番号を読み込む
	 *  @param		isLoop		// ループするかどうか
	 *  @param		transition	// 終了後の番号
	 */
	void LoadIndex( bool isLoop = false, int transition = 0);

	/*
	 *	アニメーションの再生
	 *  @param	index
	 *  @param  spped = 1.0f;
	 */
	void Play(int index, float speed = 1.0f);

public:
	/*
	 *	アニメーションを適用するモデルハンドルをセット
	 *  @param	setValue
	 */
	inline void SetModelHandle(int setValue) { animModelHandle = setValue; }

	/*
	 *	アニメーションクリップの取得
	 *  @param	index			// 取得するアニメーションクリップ番号
	 *  @retunr	AnimationClip*	// アニメーションクリップを返す
	 */
	inline AnimatorClip* GetAnimation(int index) const { return pAnimations[index]; }

	/*
	 *	@function	GetCurrentAnimation
	 *	@brief		現在のアニメーションの取得
	 *	@return		int
	 */
	inline int GetCurrentAnimation() const { return currentAnimation; }

	/*
	 *	@function	IsPlayer
	 *  @brief		アニメーションが再生中かどうか
	 *  @return		bool
	 */
	inline bool	IsPlaying() const { return isPlaying; }

	/*
	 *	@function	SetAnimIndex
	 *  @brief		名前でアニメーション番号をセット
	 */
	inline void SetAttachIndex(int animIndexs) { animIndex = animIndexs; }

};
#endif // !_ANIMATORCOMPONENT_H_

