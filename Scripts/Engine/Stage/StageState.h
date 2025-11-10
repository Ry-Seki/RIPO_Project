/*
 *	@file StageState.h
 *  @author oorui
 */
#include <memory>
#include "StageBase.h"
#include "../Manager/StageObjectManager.h"

class StageState {
private:
	std::vector<int> stageModelHandle;	// 読み込んだステージ全てを保持

	int currentStageIndex = -1;


public:
	StageState() = default;	// コンストラクタ
	~StageState() = default;	// デストラクタ

public:

	/*
	 *	ステージのモデルハンドル追加
	 */
	void AddStageModelHandle(int modelHandle) {
		if (modelHandle == -1)return;
		// モデルハンドルを追加
		stageModelHandle.push_back(modelHandle);
		currentStageIndex = static_cast<int>(stageModelHandle.size()) - 1;
	}

	/*
	 *	次のステージへ移行
	 */
	void NextStage() {
		if (currentStageIndex + 1 < (int)stageModelHandle.size())
			currentStageIndex++;
	}

	/*
	 *	ひとつ前のステージへ戻る
	 */
	void PrevStage() {
		if (currentStageIndex < 0)return;
		currentStageIndex--;
	}

	/*
	 *	全ステージ削除
	 */
	void ResetStageModelHandle() {
		for (int handle : stageModelHandle) {
			if (handle != -1) MV1DeleteModel(handle);
		}
		stageModelHandle.clear();
		currentStageIndex = -1;
		// ステージオブジェクトのフラグをfalseに
		StageObjectManager::GetInstance().ResetFlag();
	}

public:
	/*
	 * @brief 現在のステージインデックスを取得
	 */
	int GetCurrentStageIndex() const { return currentStageIndex; }

	/*
	 * @brief ステージ数を取得
	 */
	int GetStageCount() const { return static_cast<int>(stageModelHandle.size()); }

	/*
	 * @brief 現在のハンドルを取得
	 */
	int GetCurrentStageHandle() const {
		if (currentStageIndex >= 0 && currentStageIndex < (int)stageModelHandle.size())
			return stageModelHandle[currentStageIndex];
		return -1;
	}

	/*
	 * @brief ひとつ前のステージハンドルを取得
	 */
	int GetPrevStageHandle() const {
		int prev = currentStageIndex - 1;
		if (prev >= 0 && prev < (int)stageModelHandle.size())
			return stageModelHandle[prev];
		return -1;
	}

};