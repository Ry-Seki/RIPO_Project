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

	int currentStageID = -1;  // 現在の階層のID
	int previousStageID = -1; // 直前の階層のID

	
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
	}

	/*
	 *	次のステージへ移行
	 */
	void NextStage(int setID) {
		// 現在の階層IDをひとつ前の階層IDに保存
		previousStageID = currentStageID;

		// 次の階層に移動
		currentStageID = setID;
	}

	/*
	 *	ひとつ前のステージへ戻る
	 */
	void PrevStage() {
		if (previousStageID < 0) return;

		// 現在を previous に戻す
		int tmp = currentStageID;
		currentStageID = previousStageID;

		// ひとつ前の階層にする
		previousStageID = tmp;
		
	}

	/*
	 *	全ステージ削除
	 */
	void ResetStageModelHandle() {
		for (int handle : stageModelHandle) {
			if (handle != -1) MV1DeleteModel(handle);
		}
		stageModelHandle.clear();
		currentStageID = -1;
		// ステージオブジェクトのフラグをfalseに
		StageObjectManager::GetInstance().ResetFlag();
	}

public:
	/*
	 * @brief 現在のステージインデックスを取得
	 */
	int GetCurrentStageIndex() const { return currentStageID; }

	/*
	 * @brief ステージ数を取得
	 */
	int GetStageCount() const { return static_cast<int>(stageModelHandle.size()); }

	/*
	 * @brief 現在のハンドルを取得
	 */
	int GetCurrentStageHandle() const {
		if (currentStageID >= 0 && currentStageID < (int)stageModelHandle.size())
			return stageModelHandle[currentStageID];
		return -1;
	}

	/*
	 * @brief ひとつ前のステージハンドルを取得
	 */
	int GetPrevStageHandle() const {
		int prev = currentStageID - 1;
		if (prev >= 0 && prev < (int)stageModelHandle.size())
			return stageModelHandle[prev];
		return -1;
	}

	/*
	 *	指定インデックスのモデルハンドルを取得
	 *  @param	setValue
	 */
	int GetStageModelHandleAt(int setValue)const {
		if (setValue < 0 || setValue >= (int)stageModelHandle.size())return -1;
		return stageModelHandle[setValue];
	}

	/*
	 *	現在のステージインデックスを設定
	 *  @param	index
	 */
	void SetCurrentStageIndex(int index) {
		if (index >= 0 && index < (int)stageModelHandle.size())
			currentStageID = index;
	}

};