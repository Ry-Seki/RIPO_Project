/*
 *  @file Stage.h
 *  @author oorui
 */

#ifndef _STAGE_H_
#define _STAGE_H_

#include <string>
#include <vector>
#include <DxLib.h>

 /*
  *  ステージの基底クラス
  */
class Stage {
private:
	int modelHandle;		// モデルハンドル
	std::string modelPath;	// モデルのパス


public:
	Stage();
	virtual ~Stage();

	// ステージデータ読み込み
	virtual void Load(const std::string& csvPath);

	// 更新
	virtual void Update();

	// 描画
	virtual void Render();

	// 終了処理
	virtual void Execute();

	// 当たり判定の更新
	virtual void UpdateCollision();
};


#endif // !_STAGE_H_