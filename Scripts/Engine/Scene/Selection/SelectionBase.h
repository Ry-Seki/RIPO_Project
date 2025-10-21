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
 *	行動の選択肢を管理するクラス
 */
class SelectionBase {
protected:
    bool isComplete = false;        // 選択完了フラグ

public:
    /*
     *  デストラクタ
     */
    virtual ~SelectionBase() = default;

public:
    /*
     *  初期化処理
     */
    virtual void Initialize(Engine& engine) = 0;
    /*
     *  ロード済みデータのセット（コールバック）
     */
    virtual void Setup(Engine& engine) = 0;
    /*
     *  更新処理
     */
    virtual void Update(Engine& engine, float deltaTime) = 0;
    /*
     *  描画処理
     */
    virtual void Render() = 0;

public:
    /*
     *  選択完了フラグの取得
     */
    inline virtual bool IsComplete() const { return isComplete; }
};
// 別名定義
using SelectionPtr = std::shared_ptr<SelectionBase>;

#endif // !_SELECTION_BASE_H_