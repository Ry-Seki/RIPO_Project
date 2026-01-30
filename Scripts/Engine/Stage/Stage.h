/*
 *  @file Stage.h
 *  @author oorui
 */

#ifndef _STAGE_H_
#define _STAGE_H_

#include "StageBase.h"
#include <string>
#include <vector>
#include "../VecMath.h"
#include "../GameObject.h"

 // 前方宣言
// class GameObject;
class StageCollision;



/*
 *  ステージクラス
 */
class Stage : public StageBase {
private:
	std::string modelPath;						// モデルのパス
	Vector3 lightDirection;						// ライトの距離
	std::unique_ptr<StageCollision> collision;	// 当たり判定管理クラス

	Vector3 pointLightColor;			// 色
	std::vector<Vector3>pointLightPos;	// 位置
	float pointLightRange;				// 効果範囲

	GameObjectPtr player;				// プレイヤー

	struct Attenuation {
		float Atten0;
		float Atten1;
		float Atten2;
	};
	static constexpr DxLib::COLOR_F _MAP_DIF_COLOR = { 1.5f, 1.5f, 1.3f, 1.0f };	// マップ全体のライト ディフューズカラー
	static constexpr DxLib::COLOR_F _MAP_AMB_COLOR = { 0.2f, 0.25f, 0.3f, 1 };		// マップ全体のライト アンビエントカラー
	static constexpr DxLib::COLOR_F _MAP_SPC_COLOR = { 0.2f, 0.2f, 0.2f, 1 };		// マップ全体のライト スペキュラーカラー

	static constexpr Attenuation _POINT_ATTAN = { 0.0f, 0.0005f, 0.0f };
	static constexpr DxLib::COLOR_F _POINT_SPC_COLOR = { 0.1f, 0.1f, 0.1f, 1.0f };	// ポイントライト スペキュラーカラー



public:
	Stage();
	virtual ~Stage() override;

	/*
	 *	ステージデータの読み込み
	 *  @pram const int modelHandle
	 */
	void SetModelHandle(const int modelHandleBase)override;

	/*
	 *	更新
	 */
	void Update() override;

	/*
	 *	描画
	 */
	void Render() override;

	/*
	 *	終了処理
	 */
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