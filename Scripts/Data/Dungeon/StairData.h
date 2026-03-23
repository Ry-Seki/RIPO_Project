/*
 *	StairData.h
 *  @author oorui
 */

#ifndef _STAIRDATA_H_
#define _STAIRDATA_H_

#include "../../Engine/Singleton.h"
/*
 *	触れた階段の情報を保持する構造体
 */
struct StairTouchData {
	int respawnID = 0;	// リスポーン先のID
	float angle = 0;	// 自身のアングル
};

/*
 *	触れた階段の情報を保持するデータクラス
 */
class StairData : public Singleton<StairData> {
	// フレンド宣言	
	friend class Singleton<StairData>;
private:
	StairTouchData touchData;	// 現在触れている階段情報

private:
	StairData() {
		Reset();
	};
	~StairData() = default;
public:

	/*
	 *	階段接触データの設定
	 */
	void SetTouchData(const StairTouchData& data) {
		touchData = data;
	}

	/*
	 *	階段接触データの取得
	 */
	const StairTouchData& GetTouchData() const {
		return touchData;
	}

	/*
	 *	データの初期化
	 */
	void Reset() {
		touchData = {};
	}
};

#endif // !_STAIRDATA_H_
