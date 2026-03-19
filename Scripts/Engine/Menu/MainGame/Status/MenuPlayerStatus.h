/*
 *	@file	MenuPlayerStatus.h
 *	@author	Seki
 */

#ifndef _MENU_PLAYER_STATUS_H_
#define _MENU_PLAYER_STATUS_H_

#include "../../MenuBase.h"
#include "../../../UI/Sprite/Sprite.h"
#include "../../../UI/Text/TextBase.h"
#include "../../../UI/Button/UIButtonBase.h"
#include "../../../UI/EventSystem.h"
#include "../../../GameEnum.h"
#include "../../../GameConst.h"
#include "../../../../Data/PlayerStatusData.h"

#include <functional>

 // 前方宣言
class Engine;
struct PlayerStatusData;

/*
 *	@brief	プレイヤーのステータスメニュー
 */
class MenuPlayerStatus : public MenuBase {
private:
	float animTimer = 0.0f;
	int animFrame = 0;
	bool isCallback = false;
	PlayerStatusData currentStatus;
	PlayerStatusData prevStatus;
	EventSystem eventSystem;

	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<TextBase*> prevTextList;
	std::vector<TextBase*> currentTextList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;

	std::function<void()> Callback = nullptr;

public:
	/*
	 *	@brief	デストラクタ
	 */
	~MenuPlayerStatus() override {}

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	メニューを開く
	 */
	void Open() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	アニメーション等の更新
	 */
	void AnimUpdate(Engine& engine, float unscaledDeltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	メニューを閉じる
	 */
	void Close(Engine& engine) override;

private:
	/*
	 *	@brief	ボタンの押された時の処理
	 */
	void SelectButtonExecute();
	/*
	 *	@brief	現在のステータスの描画
	 */
	void CurrentStatusRender();
	/*
	 *	@brief	ステータスの比較処理
	 */
	void ComparisonStatus();
	/*
	 *	@brief	コールバックの実行
	 */
	void ExecuteCallback();
	/*
	 *	@brief	前と現在のステータステキストを振り分ける
	 */
	void SortStatusText();

public:
	/*
	 *	@brief		増加前のステータス設定
	 *	@param[in]	PlayerStatusData* setData
	 */
	inline void SetPrevStatusData(const PlayerStatusData& setData) {
		prevStatus = setData;
	}
	/*
	 *	@brief		コールバック判定の設定
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsCallback(bool setFlag) {
		isCallback = setFlag;
	}
	/*
	 *	@brief		コールバックの設定
	 *	@param[in]	std::function<void()> setCallback
	 */
	inline void SetCallback(std::function<void()> setCallback) {
		Callback = setCallback;
	}

};

#endif // !_MENU_PLAYER_STATUS_H_