/*
 *	@file	SelectionBase.h
 *	@author	Seki
 */

#ifndef _SELECTION_BASE_H_
#define _SELECTION_BASE_H_

#include <functional>
#include <memory>
#include <any>

/*
 *	行動の選択肢を管理するクラス
 */
class SelectionBase {
public:
    virtual ~SelectionBase() = default;

    // context: 任意の入力データ（例: 現在のワールドマップ情報）
    // onComplete: 選択結果（std::any）を返すコールバック
    virtual void Show(const std::any& context, const std::function<void(std::any)>& onComplete) = 0;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool IsActive() const = 0;
};
// 別名定義
using SelectionBasePtr = std::shared_ptr<SelectionBase>;

#endif // !_SELECTION_BASE_H_