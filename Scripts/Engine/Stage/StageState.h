/*
 *	@file StageState.h
 *  @author oorui
 */
#include <memory>
#include "StageBase.h"

class StageState {
private:
	std::unique_ptr<StageBase> currentStage;	// 現在のステージ
	std::unique_ptr<StageBase> prevStage;		// ひとつ前のステージ


public:
	StageState() = default;	// コンストラクタ
	~StageState() = default;	// デストラクタ

public:
	/*
	 *	現在のステージをひとつ前のステージに設定
	 *  @param setValue		// 現在のステージ
	 */
	void SetCurrentStage(std::unique_ptr<StageBase> setValue) {
		// 現在のステージを前のステージとして保存
		prevStage = std::move(currentStage);
		currentStage = std::move(setValue);
	}

	/*
	 *	前ステージを再利用できるようにする
	 */
	void RestorePreviousStage() {
		// 再利用を行う
		currentStage = std::move(prevStage);
	}


public:

	/*
	 *	現在のステージの取得
	 *  @return StageBase		現在のステージのベースを返す
	 */
	StageBase* GetCurrentStage()const { return currentStage.get(); }

	/*
	 *	ひとつ前のステージの取得
	 */
	StageBase* GetPrevStage() const { return prevStage.get(); }

};