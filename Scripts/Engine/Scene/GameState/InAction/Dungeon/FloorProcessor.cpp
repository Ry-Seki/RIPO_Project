/*
 *	@file	FloorProcessor.cpp
 *	@author	Seki
 */
 
#include "FloorProcessor.h"
#include "../../../../GameConst.h"
#include "../../ActionContext.h"
#include "../../../../Component/GravityComponent.h"
#include "../../../../Component/Character/CharacterUtility.h"
#include "../../../../GameObject/GameObjectUtility.h"
#include "../../../../Stage/StageUtility.h"
#include "../../../../Stage/StageObject/StageObjectUtility.h"
#include "../../../../Manager/CameraManager.h"
#include "../../../../Manager/StageObjectManager.h"
#include "../../../../Fade/FadeFactory.h"
#include "../../../../Fade/FadeManager.h"
#include "../../../../System/World/WorldProgressManager.h"

/*
 *	@brief	現在のフロアの片付け処理
 */
void FloorProcessor::TeardownCurrentFloor() {
	// ステージオブジェクトの振りあ
	StageObjectManager::GetInstance().ClearObject();
	// 現在のフロア内の敵のデータを設定
	ConvertEnemyData();
	// 現在残っている敵の片付け処理
	TeardownEnemy();
	// プレイヤーが持っているお宝以外を削除
	TeardownStageObject();
}
/*
 *	@brief	敵の再生成に必要なデータへ変換
 */
void FloorProcessor::ConvertEnemyData() {
	// 現在のフロアの敵データの要素数を空にする
	enemyFloorList[currentFloor].clear();
	// 敵を取得
	auto enemyList = GameObjectUtility::GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	for (auto& enemy : enemyList) {
		if (!enemy) continue;

		auto component = enemy->GetComponent<EnemyComponent>();
		if (!component) continue;

		enemyFloorList[currentFloor].push_back(component->GetSpawnEnemyID());
	}
}
/*
 *	@brief	敵の片付け処理
 */
void FloorProcessor::TeardownEnemy() {
	GameObjectList deleteEnemyList = GameObjectUtility::GetObjectByName(GameConst::_CREATE_POSNAME_ENEMY);
	for (auto& enemy : deleteEnemyList) {
		if (!enemy) continue;
		int deleteID = enemy->ID;
		CharacterUtility::RemoveCharacter(deleteID);
	}
	GameObjectList deleteBossList = GameObjectUtility::GetObjectByName(GameConst::_CREATE_POSNAME_BOSS);
	for (auto& boss : deleteBossList) {
		if (!boss) continue;
		int deleteID = boss->ID;
		CharacterUtility::RemoveCharacter(deleteID);
	}
}
/*
 *	@brief	ステージオブジェクトの片付け処理
 */
void FloorProcessor::TeardownStageObject() {
	// プレイヤーが所持しているお宝を取得
	auto holdTreasure = StageManager::GetInstance().GetLiftObject();
	int holdTreasureID = -1;
	// IDを取得
	if (holdTreasure) holdTreasureID = holdTreasure->ID;
	// ステージオブジェクトリストの取得
	auto stageObjectList = StageObjectManager::GetInstance().GetCreateObjectList();
	// プレイヤーが所持しているお宝以外を削除する
	for (int i = stageObjectList.size() - 1; i >= 0; i--) {
		GameObject* stageObject = stageObjectList[i].get();
		if (!stageObject) continue;
		// IDを取得
		int ID = stageObject->ID;
		// お宝を持っていて、かつIDが一致していたら、お宝のフラグ変更し消さない
		if (holdTreasure && ID == holdTreasureID) {
			auto component = holdTreasure->GetComponent<StageObjectBase>();
			if (!component) continue;
			// フラグ変更
			component->SetIsHold(true);
			continue;
		}else {
			StageObjectUtility::RemoveStageObject(ID);
		}
	}
}
/*
 *	@brief	次のフロアの準備処理
 */
void FloorProcessor::SetupNextFloor() {
	// フロアの再構築
	FloorData setFloorData;
	// フロアデータの取得
	floorData.TryGetFloorData(currentFloor, setFloorData);
	// そのフロアが初回かどうかで分岐
	if (setFloorData.isFirst) {
		setFloorData.isFirst = false;
	}else {
		setFloorData.enemySpawnCount = enemyFloorList[currentFloor].size();
	}
	// ボス生成フラグ
	if (dungeonProgressData.isBossDefeated) setFloorData.bossSpawnCount = 0;
	//　お宝生成数の設定
	setFloorData.treasureSpawnCount = GetSpawnTreasureIDTable().size();
	// ダンジョンクリエイターに情報を設定
	dungeonCreater.SetFloorData(setFloorData);
	// フロアを再生成
	dungeonCreater.RegenerateDungeon(
		currentFloor, enemyFloorList[currentFloor], holdTreasureObjectID, GetSpawnTreasureIDTable(), nextFloor
	);
	// フロアデータの更新
	floorData.TrySetFloorData(currentFloor, setFloorData);
	// フェードイン
	FadeBasePtr fade = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::NonStop);
	FadeManager::GetInstance().StartFade(fade);
	// 当たり判定の有効化
	GameObjectUtility::SetUseObjectColliderFlag(true);
}
/*
 *	@brief		フロアの生成
 *  @param[in]	ActionContext setContext
 *  @param[in]	bool& isStart
 */
void FloorProcessor::CreateFloor(ActionContext setContext, bool& isStart) {
	currentFloor = 0;
	enemyFloorList.resize(16);
	// データの取得
	stageData = setContext.dungeonStageData;
	floorData = setContext.dungeonFloorData;
	dungeonProgressData = WorldProgressManager::GetInstance().GetDungeonProgressData(setContext.dungeonID);
	LoadManager& load = LoadManager::GetInstance();
	// リソースの読み込み
	resourceData.LoadResourcesFromStageData(stageData, 
											dungeonProgressData.isBossDefeated,
											GetAllSpawnTreasureIDTable());

	// ロード完了時のコールバック登録
	load.SetOnComplete([this, &isStart]() {
		// フロアデータの作成
		FloorData setFloorData;
		// フロアデータの変更
		floorData.TryGetFloorData(currentFloor, setFloorData);
		setFloorData.isFirst = false;
		// ボスを生成するか判定
		if (dungeonProgressData.isBossDefeated) setFloorData.bossSpawnCount = 0;
		//　お宝生成数の設定
		setFloorData.treasureSpawnCount = GetSpawnTreasureIDTable().size();
		// ダンジョン生成クラスに必要なデータを設定
		dungeonCreater.SetDungeonData(setFloorData, resourceData);
		// ダンジョンの生成
		dungeonCreater.GenerateDungeon(currentFloor, GetSpawnTreasureIDTable(), nextFloor);
		// フロアデータの更新
		floorData.TrySetFloorData(currentFloor, setFloorData);
		// 重力
		auto player = CharacterManager::GetInstance().GetPlayer();
		GameObjectManager::GetInstance().SetObjectColliderFlag(true);
		player->GetComponent<GravityComponent>()->SetGravity(true);
		isStart = true;
	});
}
/*
 *	@brief	フロアの変更処理
 */
void FloorProcessor::ChangeFloor() {
	// 当たり判定の解除
	GameObjectUtility::SetUseObjectColliderFlag(false);
	// 現在のフロアの片付け処理
	TeardownCurrentFloor();
	// フロアの変更
	StageManager::GetInstance().NextStage(nextFloor);
	currentFloor = nextFloor;
	// 次の階層の準備処理
	SetupNextFloor();
}
/*
 *	@brief	ダンジョン終了処理
 */
void FloorProcessor::EndDungeon() {
	holdTreasureObjectID = -1;
	GameObjectUtility::SetUseObjectColliderFlag(false);
	CharacterUtility::RemoveAllCharacter();
	StageManager::GetInstance().Execute();
	StageObjectUtility::RemoveAllStageObject();
	CameraManager::GetInstance().ResetCamera();
}
/*
 *	@brief		フロア関係なしのお宝ID一覧を取得
 *  @return		std::vector<int>
 */
std::vector<int> FloorProcessor::GetAllTreasureIDTable() {
	std::vector<int> result;
	// お宝情報を取得
	auto treasureMap = stageData.GetCategory("Treasure");
	for (const auto& [key, value] : treasureMap) {
		// .で文字列を切り分ける
		auto parts = StringUtility::Split(key, '.');
		if (parts.size() != 2) continue;

		int treasureID = std::stoi(parts[1]);
		// お宝IDを追加
		result.push_back(treasureID);
	}
	return result;
}
/*
 *	@brief		フロア関係なしの生成するお宝ID一覧の取得
 *  @return		std::vector<int>
 */
std::vector<int> FloorProcessor::GetAllSpawnTreasureIDTable() {
	// 生成するお宝IDリストを取得
	std::vector<int> treasureIDList = GetAllTreasureIDTable();
	// お宝獲得状況マップを取得
	auto& getTreasureIDMap = dungeonProgressData.treasureFlagMap;
	if (getTreasureIDMap.empty()) return treasureIDList;
	// 取得済みIDを生成候補から除外
	treasureIDList.erase(
		std::remove_if(treasureIDList.begin(), treasureIDList.end(),
		[&](int treasureID) {
		auto itr = getTreasureIDMap.find(treasureID);
		return itr != getTreasureIDMap.end() && itr->second;
	}), treasureIDList.end());

	return treasureIDList;	// 結果を反映した物を返す
}
/*
 *	@brief		フロアごとのTreasureID一覧を取得
 *	@return		std::vector<int>
 */
std::vector<int> FloorProcessor::GetTreasureIDTable() {
	std::vector<std::vector<int>> result;

	auto treasureMap = stageData.GetCategory("Treasure");
	// JSONのキーから数字を取り出す
	for (const auto& [key, value] : treasureMap) {
		// .で文字列を切り分ける
		auto parts = StringUtility::Split(key, '.');
		if (parts.size() != 2) continue;
		// 一つ目はフロア数
		int floor = std::stoi(parts[0]);
		// 二つ目はお宝ID
		int treasureID = std::stoi(parts[1]);
		// floor 番目の要素が存在しないか判定
		if (result.size() <= static_cast<size_t>(floor)) result.resize(floor + 1);

		result[floor].push_back(treasureID);
	}
	return result[currentFloor];
}
/*
 *	@brief		フロアごとに生成するお宝IDデータのみを渡す
 *	@return		std::vector<int>
 */
std::vector<int> FloorProcessor::GetSpawnTreasureIDTable() {
	// 生成するお宝IDリストを取得
	std::vector<int> treasureIDList = GetTreasureIDTable();
	// お宝獲得状況マップを取得
	auto& getTreasureIDMap = dungeonProgressData.treasureFlagMap;
	if(getTreasureIDMap.empty()) return treasureIDList;
	// 取得済みIDを生成候補から除外
	treasureIDList.erase(
		std::remove_if(treasureIDList.begin(),treasureIDList.end(),
		[&](int treasureID) {
		auto itr = getTreasureIDMap.find(treasureID);
		return itr != getTreasureIDMap.end() && itr->second;
		}), treasureIDList.end());

	return treasureIDList;	// 結果を反映した物を返す
}
/*
 *	@brief		プレイヤーが所持しているお宝IDの取得
 *	@return		int
 */
int FloorProcessor::GetHoldTreasureID() const {
	// プレイヤーが所持しているお宝を取得
	auto holdTreasure = StageManager::GetInstance().GetLiftObject();
	if (!holdTreasure) return -1;

	auto component = holdTreasure->GetComponent<StageObjectBase>();
	if (!component) return -1;

	return component->GetTreasureID();
}