/*
 *	@file	MiniGameSokoban.cpp
 *	@author	Seki
 */

#include "MiniGameSokoban.h"
#include "SokobanMapManager.h"
/*
 *  @brief ミニゲーム開始時処理
 */
void MiniGameSokoban::Open() {
    MiniGameBase::Open();

    // マップ読み込み
    SokobanMapManager mapManager;
    if(!mapManager.LoadMapList("Data/MiniGame/Sokoban")) return;

    std::string mapFile = mapManager.GetRandomMap();
    createMap.LoadMapFromFile(mapFile);
    initMap = createMap.GetMap();
    map = createMap.GetMap();

    // goalMap を初期化
    goalMap.assign(map.size(), std::vector<bool>(map[0].size(), false));

    // マップの初期化
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            if (map[y][x] == TileType::Goal) {
                goalMap[y][x] = true;
                map[y][x] = TileType::Empty;
            }
            if (map[y][x] == TileType::Player) {
                playerX = x;
                playerY = y;
                playerDrawX = x * TILE_SIZE;
                playerDrawY = y * TILE_SIZE;
            }
        }
    }
    // 箱のリスト化
    ParseBoxesFromMap();
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
                box.drawX = x * TILE_SIZE;
                box.drawY = y * TILE_SIZE;
                box.moveTimer = 1.0f;
                boxList.push_back(box);
            }
        }
    }
}
/*
 *  @brief      座標指定の箱の取得
 *  @return     SokobanBox*
 */
SokobanBox* MiniGameSokoban::GetBox(int x, int y) {
    for (auto& b : boxList) {
        if (b.x == x && b.y == y) return &b;
    }
    return nullptr;
}
/*
 * @brief 入力受付と補間更新
 */
void MiniGameSokoban::Update(Engine& engine, float dt) {
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
    UpdateInterp(dt);

    // 補間が完全終了してからクリア判定
    if (!IsMoving() && IsClear()) {
        Close();
    }
}
/*
 * @brief 補間中判定
 */
bool MiniGameSokoban::IsMoving() const {
    if (moveTimer < 1.0f) return true;
    for (const auto& b : boxList) {
        if (b.moveTimer < 1.0f) return true;
    }
    return false;
}
/*
 *  @brief  プレイヤー移動開始（即時適応）
 */
void MiniGameSokoban::StartPlayerMove(int dx, int dy) {
    int nx = playerX + dx;
    int ny = playerY + dy;

    // 範囲外
    if (ny < 0 || ny >= (int)map.size()) return;
    if (nx < 0 || nx >= (int)map[0].size()) return;

    TileType dest = map[ny][nx];

    // 壁は無理
    if (dest == TileType::Wall) return;

    // 箱なら押す必要あり
    if (dest == TileType::Box) {
        if (!TryPushBox(nx, ny, dx, dy)) return;
    }

    // プレイヤーロジック更新
    map[playerY][playerX] = TileType::Empty;
    map[ny][nx] = TileType::Player;

    playerX = nx;
    playerY = ny;

    // 補間開始
    moveDX = dx;
    moveDY = dy;
    moveTimer = 0.0f;
}
/*
 *  @brief  箱を押す + 補間準備
 */
bool MiniGameSokoban::TryPushBox(int bx, int by, int dx, int dy) {
    int nbx = bx + dx;
    int nby = by + dy;

    // 範囲外
    if (nby < 0 || nby >= (int)map.size()) return false;
    if (nbx < 0 || nbx >= (int)map[0].size()) return false;

    if (map[nby][nbx] == TileType::Wall) return false;
    if (map[nby][nbx] == TileType::Box) return false;

    // ロジック移動
    map[by][bx] = TileType::Empty;
    map[nby][nbx] = TileType::Box;

    // 該当箱を補間開始
    if (auto box = GetBox(bx, by)) {
        box->x = nbx;
        box->y = nby;
        box->moveDX = dx;
        box->moveDY = dy;
        box->moveTimer = 0.0f;
        box->isMoving = true;
    }

    return true;
}
/*
 *  @brief  補間更新
 */
void MiniGameSokoban::UpdateInterp(float dt) {
    // プレイヤー補間
    if (moveTimer < 1.0f) {
        moveTimer += dt / MOVE_DURATION;
        if (moveTimer > 1.0f) moveTimer = 1.0f;

        // 滑らかな移動
        float e = EaseInOut(moveTimer);

        playerDrawX = (playerX - moveDX) * TILE_SIZE * (1 - e)
            + playerX * TILE_SIZE * e;
        playerDrawY = (playerY - moveDY) * TILE_SIZE * (1 - e)
            + playerY * TILE_SIZE * e;
    }

    // 箱補間
    for (auto& b : boxList) {
        if (b.moveTimer < 1.0f) {
            b.moveTimer += dt / MOVE_DURATION;
            if (b.moveTimer > 1.0f) b.moveTimer = 1.0f;

            float e = EaseInOut(b.moveTimer);

            b.drawX = (b.x - b.moveDX) * TILE_SIZE * (1 - e)
                + b.x * TILE_SIZE * e;
            b.drawY = (b.y - b.moveDY) * TILE_SIZE * (1 - e)
                + b.y * TILE_SIZE * e;
        }
    }
}
/*
 *  @brief  リセット処理
 */
void MiniGameSokoban::Reset() {
    map = initMap;

    // goalMap 再構築
    goalMap.assign(map.size(), std::vector<bool>(map[0].size(), false));

    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            if (map[y][x] == TileType::Goal) {
                goalMap[y][x] = true;
                map[y][x] = TileType::Empty;
            }
            if (map[y][x] == TileType::Player) {
                playerX = x;
                playerY = y;
                playerDrawX = x * TILE_SIZE;
                playerDrawY = y * TILE_SIZE;
            }
        }
    }

    // 箱リスト再構築
    ParseBoxesFromMap();

    // 補間停止
    moveTimer = 1.0f;
    moveDX = moveDY = 0;
}
/*
 *  @brief   描画処理
 */
void MiniGameSokoban::Render() {
    // タイル描画（床・壁）
    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            int l = x * TILE_SIZE;
            int t = y * TILE_SIZE;
            int r = l + TILE_SIZE;
            int b = t + TILE_SIZE;

            auto type = map[y][x];

            int col = GetColor(200, 200, 200); // 床
            if (type == TileType::Wall)
                col = GetColor(80, 80, 80);    // 壁

            DrawBox(l, t, r, b, col, TRUE);

            // ★ Goal を描く（map ではなく goalMap を参照！）
            if (goalMap[y][x]) {
                DrawBox(l + 4, t + 4, r - 4, b - 4, GetColor(0, 180, 0), TRUE);
            }
        }
    }

    // 箱描画（ゴール上なら色を変える）
    for (const auto& box : boxList) {
        int col = GetColor(160, 120, 40);

        // ゴールと重なっている場合、色を変更
        if (goalMap[box.y][box.x]) {
            col = GetColor(255, 200, 0);
        }

        DrawBox(
            (int)box.drawX,
            (int)box.drawY,
            (int)box.drawX + TILE_SIZE,
            (int)box.drawY + TILE_SIZE,
            col, TRUE
        );
    }

    // プレイヤー描画
    DrawBox(
        (int)playerDrawX,
        (int)playerDrawY,
        (int)playerDrawX + TILE_SIZE,
        (int)playerDrawY + TILE_SIZE,
        GetColor(0, 0, 255),
        TRUE
    );
}
/*
 *  @brief  ミニゲーム終了処理
 */
void MiniGameSokoban::Close() {
    MiniGameBase::Close();
    score = 25;
}
/*
 *	@brief		クリア判定
 *	@return		bool
 */
bool MiniGameSokoban::IsClear() {
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {

            if (goalMap[y][x]) {
                // Goal 上に Box がなければ未達成
                if (map[y][x] != TileType::Box)
                    return false;
            }

        }
    }
    return true;
}