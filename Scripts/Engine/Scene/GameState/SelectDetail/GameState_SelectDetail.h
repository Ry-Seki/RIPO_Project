/*
 *	@brief	GameState_SelectDetail.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_SELECT_DETAIL_H_
#define _GAME_STATE_SELECT_DETAIL_H_

#include "../GameStateBase.h"

#include <vector>
#include <memory>

// 前方宣言
class SelectDetailBase;

class GameState_SelectDetail : public GameStateBase {
public:
	using SelectDetailPtr = std::shared_ptr<SelectDetailBase>;

private:
	std::vector<SelectDetailPtr> selectDetailList;
	SelectDetailPtr currentDetail;
	SelectDetailPtr prevDetail;

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
	 *	@brief	片付け処理
	 */
	void Teardown() override;

public:
	/*
	 *	@brief	ロード済みデータのセット(コールバック)
	 */
	void SetupData();

public:
	

};

#endif // !_GAME_STATE_SELECT_DETAIL_H_