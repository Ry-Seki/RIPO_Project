/*
 *  @file Stage.h
 *  @author oorui
 */

#ifndef _STAGE_H_
#define _STAGE_H_

#include "StageBase.h"
#include <string>
#include <vector>
#include "../Load/LoadManager.h"
 /*
  *  ステージクラス
  */
class Stage : public StageBase {
private:
	std::string modelPath;	// モデルのパス

public:
	Stage();
	virtual ~Stage() override;

	// ステージデータ読み込み
	void Load(const std::shared_ptr<LoadModel>& model) override;

	// 更新
	void Update() override;

	// 描画
	void Render() override;

	// 終了処理
	void Execute() override;

	// 当たり判定の更新
	void UpdateCollision() override;
};


#endif // !_STAGE_H_