/*
 *	@file	SelectionBase.h
 *	@author	Seki
 */

#ifndef _SELECTION_BASE_H_
#define _SELECTION_BASE_H_

#include <functional>
#include <memory>

// 前方宣言
class Engine;

/*
 *	@brief  行動の選択肢を管理するクラス
 */
class SelectionBase {
protected:
    bool isComplete = false;        // 選択完了フラグ

public:
    /*
     *  @brief  デストラクタ
     */
    virtual ~SelectionBase() = default;

public:
    /*
     *  @brief  初期化処理
     */
    virtual void Initialize(Engine& engine) = 0;
    /*
     *  @brief  準備前処理
     */
    virtual void Setup(Engine& engine) = 0;
    /*
     *  @brief  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime) = 0;
    /*
     *  @brief  描画処理
     */
    virtual void Render() = 0;
    /*
     *  @brief  片付け処理
     */
    virtual void Teardown() {}

public:
    /*
     *  @brief  選択完了フラグの取得
     */
    inline virtual bool IsComplete() const { return isComplete; }
};
// 別名定義
using SelectionPtr = std::shared_ptr<SelectionBase>;

#endif // !_SELECTION_BASE_H_