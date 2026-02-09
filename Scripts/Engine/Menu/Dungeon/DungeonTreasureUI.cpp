#include "DungeonTreasureUI.h"
#include "../../Load/LoadManager.h"
#include "../../Load/JSON/LoadJSON.h"
#include "../../Engine.h"

/*
 *	デストラクタ
 */
DungeonTreasureUI::~DungeonTreasureUI() {
}

/*
 *	初期化処理
 */
void DungeonTreasureUI::Initialize(Engine& engine) {
	auto& load = LoadManager::GetInstance();
	auto treasureUIJSON = load.LoadResource<LoadJSON>(_TREASUREUIDATA_FILEPATH);
	if (!treasureUIJSON) return;

	load.SetOnComplete([this, treasureUIJSON]() {
		// JSONデータ取得
		const auto& json = treasureUIJSON->GetData();

		// TreasureUI階層がなければ終了
		if (!json.contains(_TRESUREUIJSON_NAME)) return;

		// TreasureUI を走査
		for (auto& [idStr, value] : json[_TRESUREUIJSON_NAME].items()) {

			// ID文字列をintに変換
			int id = std::stoi(idStr);

			// ImagePath を取得
			std::string path = value.value("ImagePath", "");
			if (path.empty()) continue;

			// DxLibで画像ロード
			int graph = LoadGraph(path.c_str());
			if (graph == -1) continue;

			// IDと画像ハンドルを対応表に登録
			uiGraphTable[id] = graph;
		}
		});

}

/*
 *	更新処理
 */
void DungeonTreasureUI::Update(Engine& engine, float deltaTime) {
}

/*
 *	@brief	メニューを開く
 */
void DungeonTreasureUI::Open() {
	MenuBase::Open();
	SetIsVisible(true);
}

/*
 *	@brief	メニューを閉じる
 */
void DungeonTreasureUI::Close(Engine& engine) {
	MenuBase::Close(engine);
	SetIsVisible(false);

}
/*
 *	描画処理
 */
void DungeonTreasureUI::Render() {
	// 表示対象がなければ描画しない
	if (currentTreasureID == -1) return;

	// 画像ハンドル取得
	int graph = uiGraphTable[currentTreasureID];

	// DxLibのVectorに変換
	VECTOR pos = ToVECTOR(drawWorldPos);

	// 3Dビルボード描画
	DrawBillboard3D(
		pos,		// 描画位置
		0.5f,		// 描画する画像の中心座標
		0.5f,		// 描画する画像の中心座標
		300.0f,     // 描画する画像のサイズ
		0.0f,		// 描画する画像の回転角度
		graph,      // 画像ハンドル
		TRUE        // 透明度有効
	);
}

/*
 *	指定したTreasureIDのUIを表示する
 *  @param	int treasureID
 */
void DungeonTreasureUI::ShowTreasureUI(int treasureID, const Vector3& worldPos) {

	// 対応するUIが無ければ何もしない
	if (!uiGraphTable.contains(treasureID)) return;

	// 表示中のIDを保存
	currentTreasureID = treasureID;

	// 表示位置
	drawWorldPos = worldPos + Vector3(0.0f, 350.0f, 0.0f);

}

/*
 *	UIを非表示にする
 */
void DungeonTreasureUI::HideTreasureUI() {
	currentTreasureID = -1;
	
}
