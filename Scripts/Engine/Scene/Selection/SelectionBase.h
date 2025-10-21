/*
 *	@file	SelectionBase.h
 *	@author	Seki
 */

#ifndef _SELECTION_BASE_H_
#define _SELECTION_BASE_H_

#include <functional>
#include <memory>

/*
 *	行動の選択肢を管理するクラス
 */
class SelectionBase {
protected:
    bool isActive = false;

public:
    /*
     *  デストラクタ
     */
    virtual ~SelectionBase() = default;

public:
    /*
     *  初期化処理
     */
    virtual void Initialize() = 0;
    /*
     *  ロード済みデータのセット（コールバック）
     */
    virtual void Setup() = 0;
    /*
     *  更新処理
     */
    virtual void Update(float deltaTime) = 0;
    /*
     *  描画処理
     */
    virtual void Render() = 0;

public:
    virtual bool IsActive() const = 0;
};
// 別名定義
using SelectionPtr = std::shared_ptr<SelectionBase>;

#endif // !_SELECTION_BASE_H_