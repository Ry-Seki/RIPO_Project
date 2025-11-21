/*
 *	@file	MiniGameSokoban.cpp
 *	@author	Seki
 */

#include "MiniGameSokoban.h"

/*
 *	@brief	ミニゲーム開始時処理
 */
void MiniGameSokoban::Open() {
    MiniGameBase::Open();
    createMap.LoadMapFromFile("Data/MiniGame/Sokoban/SokobanMap1.txt");
    map = createMap.GetMap();

    // プレイヤーの位置を検索
    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            if (map[y][x] == TileType::Player) {
                playerX = x;
                playerY = y;
            }
        }
    }
}
/*
 *	@brief	更新処理
 */
void MiniGameSokoban::Update(Engine& engine, float deltaTime) {
    // 入力受付
    int dx = 0, dy = 0;
    if (CheckHitKey(KEY_INPUT_W)) dy = -1;
    else if (CheckHitKey(KEY_INPUT_S)) dy = 1;
    else if (CheckHitKey(KEY_INPUT_A)) dx = -1;
    else if (CheckHitKey(KEY_INPUT_D)) dx = 1;

    if (dx != 0 || dy != 0) {
        MovePlayer(dx, dy);
    }
}
/*
 *	@brief	描画処理
 */
void MiniGameSokoban::Render() {
    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            int left = x * tileSize;
            int top = y * tileSize;
            int right = left + tileSize;
            int bottom = top + tileSize;

            switch (map[y][x]) {
                case TileType::Empty:
                    DrawBox(left, top, right, bottom, GetColor(200, 200, 200), TRUE);
                    break;
                case TileType::Wall:
                    DrawBox(left, top, right, bottom, GetColor(100, 100, 100), TRUE);
                    break;
                case TileType::Box:
                    DrawBox(left, top, right, bottom, GetColor(150, 75, 0), TRUE);
                    break;
                case TileType::Goal:
                    DrawBox(left, top, right, bottom, GetColor(0, 255, 0), TRUE);
                    break;
                case TileType::Player:
                    DrawBox(left, top, right, bottom, GetColor(0, 0, 255), TRUE);
                    break;
            }
        }
    }
    // クリア判定
    if (IsClear()) Close();
}
/*
 *	@brief	ミニゲーム終了時処理
 */
void MiniGameSokoban::Close() {
    MiniGameBase::Close();
    score = 25;
}
/*
 *	@brief		プレイヤーの移動処理
 *  @param[in]	int dx
 *  @param[in]	int dy
 */
void MiniGameSokoban::MovePlayer(int dx, int dy) {
    int nx = playerX + dx;
    int ny = playerY + dy;

    if (nx < 0 || ny < 0 || ny >= (int)map.size() || nx >= (int)map[0].size()) return;
    TileType dest = map[ny][nx];

    if (dest == TileType::Wall) return;
    if (dest == TileType::Box) {
        int bx = nx + dx;
        int by = ny + dy;
        if (bx < 0 || by < 0 || by >= (int)map.size() || bx >= (int)map[0].size()) return;
        if (map[by][bx] != TileType::Empty && map[by][bx] != TileType::Goal) return;
        map[by][bx] = TileType::Box;
    }

    // プレイヤー移動
    map[playerY][playerX] = TileType::Empty;
    map[ny][nx] = TileType::Player;
    playerX = nx;
    playerY = ny;
}
/*
 *	@brief		クリア判定
 *	@return		bool
 */
bool MiniGameSokoban::IsClear() {
    for (int y = 0; y < (int)map.size(); ++y)
        for (int x = 0; x < (int)map[y].size(); ++x)
            if (map[y][x] == TileType::Goal) return false;
    return true;
}