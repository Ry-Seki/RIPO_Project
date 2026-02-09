/*
 *	@file	MiniGameSokoban.cpp
 *	@author	Seki
 */

#include "MiniGameSokoban.h"
#include "SokobanMapManager.h"
#include <DxLib.h>

/*
 *  @brief ミニゲーム開始時処理
 */
void MiniGameSokoban::Open() {
    MiniGameBase::Open();
    // マップ読み込み
    SokobanMapManager mapManager;
    if(!mapManager.LoadMapList(GetMiniGamePath())) return;
    // マップをランダムに取得する
    std::string mapFile = mapManager.GetRandomMap();
    if(!createMap.LoadMapFromFile(mapFile)) return;

    // マップを設定
    initMap = createMap.GetMap();
    map = createMap.GetMap();

    // goalMap を初期化
    goalMap.assign(map.size(), std::vector<bool>(map[0].size(), false));

    //マップの初期化処理
    InitializeMap();
    // 箱のリスト化
    ParseBoxesFromMap();
}
/*
 *	@brief		マップの初期化処理
 */
void MiniGameSokoban::InitializeMap() {
    if (map.empty()) return;
    // マップの初期化
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            if (map[y][x] == TileType::Goal) {
                goalMap[y][x] = true;
                map[y][x] = TileType::Empty;
            }
            if (map[y][x] == TileType::Player) {
                player.x = x;
                player.y = y;
                player.drawX = x * _TILE_SIZE;
                player.drawY = y * _TILE_SIZE;
            }
        }
    }
}
/*
 *  @brief  箱リストをマップから抽出
 */
void MiniGameSokoban::ParseBoxesFromMap() {
    boxList.clear();
    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            if (map[y][x] == TileType::Box) {
                SokobanBox box;
                box.x = x;
                box.y = y;
                box.drawX = x * _TILE_SIZE;
                box.drawY = y * _TILE_SIZE;
                box.moveTimer = 1.0f;
                boxList.push_back(box);
            }
        }
    }
}
/*
 *  @brief      更新処理
 */
void MiniGameSokoban::Update(float deltaTime) {
    // R でリセット（補間中でもリセット可能）
    if (CheckHitKey(KEY_INPUT_R)) {
        Reset();
        return;
    }

    // 補間中は入力禁止
    if (!IsMoving()) {
        int dx = 0, dy = 0;
        if (CheckHitKey(KEY_INPUT_W)) dy = -1;
        else if (CheckHitKey(KEY_INPUT_S)) dy = 1;
        else if (CheckHitKey(KEY_INPUT_A)) dx = -1;
        else if (CheckHitKey(KEY_INPUT_D)) dx = 1;

        if (dx != 0 || dy != 0) {
            StartPlayerMove(dx, dy);
        }
    }

    // 補間更新
    UpdateInterp(deltaTime);

    // 補間が完全終了してからクリア判定
    if (!IsMoving() && IsClear()) {
        Close();
    }
}
/*
 *  @brief      プレイヤー移動開始（即時適応）
 *  @param[in]  int dx, dy  移動方向
 */
void MiniGameSokoban::StartPlayerMove(int dx, int dy) {
    int nextPosX = player.x + dx;
    int nextPosY = player.y + dy;

    // 範囲外
    if (nextPosY < 0 || nextPosY >= (int)map.size()) return;
    if (nextPosX < 0 || nextPosX >= (int)map[0].size()) return;

    TileType dest = map[nextPosY][nextPosX];

    // 壁は無理
    if (dest == TileType::Wall) return;

    // 箱なら押す必要あり
    if (dest == TileType::Box) {
        if (!TryPushBox(nextPosX, nextPosY, dx, dy)) return;
    }

    // プレイヤー座標更新
    map[player.y][player.x] = TileType::Empty;
    map[nextPosY][nextPosX] = TileType::Player;

    player.x = nextPosX;
    player.y = nextPosY;

    // 補間開始
    player.moveDX = dx;
    player.moveDY = dy;
    player.moveTimer = 0.0f;
}
/*
 *  @brief      箱を押し、補間準備処理
 *  @param[in]  int bx, by　箱の座標
 *  @param[in]  int dx, dy  移動方向
 *  @return     bool
 */
bool MiniGameSokoban::TryPushBox(int bx, int by, int dx, int dy) {
    // 移動先の座標
    int nextboxX = bx + dx;
    int nextboxY = by + dy;

    // 範囲外
    if (nextboxY < 0 || nextboxY >= (int)map.size()) return false;
    if (nextboxX < 0 || nextboxX >= (int)map[0].size()) return false;

    if (map[nextboxY][nextboxX] == TileType::Wall) return false;
    if (map[nextboxY][nextboxX] == TileType::Box) return false;

    // ロジック移動
    map[by][bx] = TileType::Empty;
    map[nextboxY][nextboxX] = TileType::Box;

    // 該当箱を補間開始
    if (auto box = GetBox(bx, by)) {
        box->x = nextboxX;
        box->y = nextboxY;
        box->moveDX = dx;
        box->moveDY = dy;
        box->moveTimer = 0.0f;
        box->isMoving = true;
    }

    return true;
}
/*
 *  @brief      補間移動更新処理
 */
void MiniGameSokoban::UpdateInterp(float deltaTime) {
    // プレイヤー補間
    if (player.moveTimer < 1.0f) {
        player.moveTimer += deltaTime / _MOVE_DURATION;

        // 上限処理
        if (player.moveTimer > 1.0f) player.moveTimer = 1.0f;

        // イージング移動
        float easeMove = EaseInOut(player.moveTimer);

        player.drawX = (player.x - player.moveDX) * _TILE_SIZE * (1 - easeMove) + player.x * _TILE_SIZE * easeMove;
        player.drawY = (player.y - player.moveDY) * _TILE_SIZE * (1 - easeMove) + player.y * _TILE_SIZE * easeMove;
    }

    // 箱補間
    for (auto& box : boxList) {
        if (box.moveTimer < 1.0f) {
            box.moveTimer += deltaTime / _MOVE_DURATION;
            if (box.moveTimer > 1.0f) box.moveTimer = 1.0f;

            // イージング移動
            float easeMove = EaseInOut(box.moveTimer);

            box.drawX = (box.x - box.moveDX) * _TILE_SIZE * (1 - easeMove) + box.x * _TILE_SIZE * easeMove;
            box.drawY = (box.y - box.moveDY) * _TILE_SIZE * (1 - easeMove) + box.y * _TILE_SIZE * easeMove;
        }
    }
}
/*
 *  @brief      マップリセット処理
 */
void MiniGameSokoban::Reset() {
    // 初期時点のマップに設定
    map = initMap;
    // goalMap再構築
    goalMap.assign(map.size(), std::vector<bool>(map[0].size(), false));
    // マップの再構築
    InitializeMap();
    // 箱リスト再構築
    ParseBoxesFromMap();
    // 補間停止
    player.moveTimer = 1.0f;
    player.moveDX = player.moveDY = 0;
}
/*
 *  @brief      描画処理
 */
void MiniGameSokoban::Render() {
    // タイル描画
    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            int left = x * _TILE_SIZE;
            int top = y * _TILE_SIZE;
            int right = left + _TILE_SIZE;
            int bottom = top + _TILE_SIZE;

            auto type = map[y][x];

            int color = GetColor(200, 200, 200); // 床
            if (type == TileType::Wall) color = GetColor(80, 80, 80);    // 壁

            DrawBox(left, top, right, bottom, color, TRUE);

            // Goalの描画(分かりやすいように少し小さく描画)
            if (goalMap[y][x]) DrawBox(left + 4, top + 4, right - 4, bottom - 4, GetColor(0, 180, 0), TRUE);
        }
    }

    // 箱描画
    for (const auto& box : boxList) {
        // 色の設定
        int color = GetColor(160, 120, 40);

        // ゴールと重なっている場合、色を変更
        if (goalMap[box.y][box.x]) color = GetColor(255, 200, 0);

        DrawBox(
            (int)box.drawX,
            (int)box.drawY,
            (int)box.drawX + _TILE_SIZE,
            (int)box.drawY + _TILE_SIZE,
            color, TRUE
        );
    }

    // プレイヤー描画
    DrawBox(
        (int)player.drawX,
        (int)player.drawY,
        (int)player.drawX + _TILE_SIZE,
        (int)player.drawY + _TILE_SIZE,
        GetColor(0, 0, 255),
        TRUE
    );
}
/*
 *  @brief      ミニゲーム終了処理
 */
void MiniGameSokoban::Close() {
    MiniGameBase::Close();
    score = 25;
}
/*
 *	@brief		難易度からフォルダのパスに変更
 *	@param[in]	GameEnum::MiniGameLevel setLevel
 *	@return		std::string
 */
std::string MiniGameSokoban::GetMiniGamePath() const {
    std::string path;
    switch (gameLevel) {
        case GameEnum::MiniGameLevel::Easy:
            path = "Easy";
            break;
        case GameEnum::MiniGameLevel::Normal:
            path = "Normal";
            break;
        case GameEnum::MiniGameLevel::Hard:
            path = "Hard";
            break;
        default:
            return "";
    }
    return _SOKOBAN_PATH + path;
}
/*
 *  @brief      座標指定の箱の取得
 *  @param[in]	int x, y		    箱の座標
 *  @return     SokobanBox*
 */
MiniGameSokoban::SokobanBox* MiniGameSokoban::GetBox(int x, int y) {
    for (auto& box : boxList) {
        if (box.x == x && box.y == y) return &box;
    }
    return nullptr;
}
/*
 *  @brief      補間移動中か判定
 *  @return     bool
 */
bool MiniGameSokoban::IsMoving() const {
    if (player.moveTimer < 1.0f) return true;
    for (const auto& box : boxList) {
        if (box.moveTimer < 1.0f) return true;
    }
    return false;
}
/*
 *	@brief		クリア判定
 *	@return		bool
 */
bool MiniGameSokoban::IsClear() {
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            // Goal上にBoxがあるか確認
            if (goalMap[y][x]) {
                if (map[y][x] != TileType::Box) return false;
            }
        }
    }
    return true;
}