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
#include "../VecMath.h"
#include "../GameConst.h"
#include "../Manager/StageManager.h"
#include "StageObject/StageCollision.h"
 /*
  *  ステージクラス
  */
class Stage : public StageBase {
private:
	std::string modelPath;				// モデルのパス
	Vector3 lightDirection;				// ライトの距離

	Vector3 pointLightColor;			// 色
	std::vector<Vector3>pointLightPos;	// 位置
	float pointLightRange;				// 効果範囲

	GameObjectPtr player;				// プレイヤー

	static constexpr DxLib::COLOR_F _MAP_DIF_COLOR = { 1.5f, 1.5f, 1.3f, 1.0f };	// マップ全体のライト ディフューズカラー
	static constexpr DxLib::COLOR_F _MAP_AMB_COLOR = { 0.2f, 0.25f, 0.3f, 1 };		// マップ全体のライト アンビエントカラー
	static constexpr DxLib::COLOR_F _MAP_SPC_COLOR = { 0.2f, 0.2f, 0.2f, 1 };		// マップ全体のライト スペキュラーカラー

	struct Attenuation {
		float Atten0;
		float Atten1;
		float Atten2;
	};
	static constexpr Attenuation _POINT_ATTAN = { 0.0f, 0.0005f, 0.0f };
	static constexpr DxLib::COLOR_F _POINT_SPC_COLOR = { 0.1f, 0.1f, 0.1f, 1.0f };	// ポイントライト スペキュラーカラー

	static constexpr float _CHARACTER_MOVEVEC_MIN = 0.01f;		// キャラクターの移動量の最小値
	static constexpr float _HALF = 0.5f;						// 半分
	static constexpr float _POLYGON_HEIGHT = 0.9f;

	std::unique_ptr<StageCollision> collisionController;
public:
	Stage();
	virtual ~Stage() override;

	/*
	 *	ステージデータの読み込み
	 *  @pram const int modelHandle
	 */
	void SetModelHandle(const int modelHandleBase)override;

	// 更新
	void Update() override;

	// 描画
	void Render() override;

	// ステージの当たり判定の描画
	void StageColliderRenderer(GameObject* other, Vector3 MoveVec) override;

	// 終了処理
	void Execute() override;



	/*
	 * @brief ステージの当たり判定を更新
	 * @param position     オブジェクト位置
	 * @param MoveVec      移動ベクトル
	 */
	void UpdateCollision(GameObject* other, Vector3 MoveVec) override;

public:
	/*
	 * ステージのライトの設定
	 */
	void LightSettings();



public:
	/*
	 *	モデルハンドルの取得
	 */
	int GetModelHandle() const { return modelHandle; }

	/*
	 *	ライトの変更
	 */
	void SetPointLightPositions(const std::vector<Vector3>& positions) {
		pointLightPos = positions;
	}


};


#endif // !_STAGE_H_