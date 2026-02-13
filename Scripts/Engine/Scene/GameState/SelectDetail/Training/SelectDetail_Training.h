/*
 *	@brief	SelectDetail_Training.h
 *	@author	Seki
 */

#ifndef _SELECT_DETAIL_TRAINING_H_
#define _SELECT_DETAIL_TRAINING_H_

#include "../SelectDetailBase.h"
#include "../../../../GameEnum.h"

/*
 *	@brief	トレーニングの内容を選択するクラス
 */
class SelectDetail_Training : public SelectDetailBase {

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize() override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

private:
	/*
	 *	@brief		トレーニング内容決定
	 *	@param[in]	GameEnum::PlayerStatusType type
	 */
	void DecideTrainingType(GameEnum::PlayerStatusType type);
};

#endif // !_SELECT_DETAIL_TRAINING_H_