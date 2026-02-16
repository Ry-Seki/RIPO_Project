/*
 *	@file	DungeonTreasureUI.h
 *  @author oorui
 */

#ifndef _DUNGEONTREASUREUI_H_
#define _DUNGEONTREASUREUI_H_

#include "../MenuBase.h"
#include "../../VecMath.h"

#include <memory>
#include <vector>
#include <unordered_map>
class Engine;

/*
 *	ダンジョン中のお宝の必要筋力値UIを出す
 */
class DungeonTreasureUI : public MenuBase {
private:


	std::unordered_map<int, int> uiGraphTable;	// TreasureID と 画像ハンドルの対応
	int currentTreasureID = -1;					// 表示中のお宝のID
	Vector3 drawWorldPos;						// 表示位置のワールド座標
	int ownerObjectID = -1;						// 表示を許可されているTreasure
	static constexpr const char* _TREASUREUIDATA_FILEPATH = "Data/UI/Dungeon/Treasure/TreasureUI.json";	// お宝UIのデータのパス
	static constexpr const char* _TRESUREUIJSON_NAME = "TreasureUI";		// お宝のjsonの階層名

public:

	/*
	 *	デストラクタ
	 */
	~DungeonTreasureUI()override;

public:
	/*
	 *	初期化処理
	 */
	void Initialize(Engine& engine)override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;

	/*
	 *	@brief	メニューを閉じる
	 */
	void Close(Engine& engine) override;

	/*
	 *	更新処理
	 */
	void Update(Engine& engine, float deltaTime)override;
	/*
	 *	描画処理
	 */
	void Render()override;

public:
	/*
	 *	指定したTreasureIDのUIを表示する
	 *  @param	int treasureID			お宝の名前
	 *  @param	const Vector3& worldPos 表示したいワールド座標
	 */
	void ShowTreasureUI(int treasureID, int ownerID, const Vector3& worldPos);

	/*
	 *	UIを非表示にする
	 */
	void HideTreasureUI(int ownerID);

	/*
	 *	表示中
	 */
	int GetOwnerObjectID() const { return ownerObjectID; }
};


#endif // !_DUNGEONTREASUREUI_H_

