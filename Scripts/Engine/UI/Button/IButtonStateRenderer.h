/*
 *	@file	IButtonStateRenderer.h
 *	@author	Seki
 */

#ifndef _IBUTTON_STATE_RENDERER_H_
#define _IBUTTON_STATE_RENDERER_H_

#include <string>

// 前方宣言
struct Rect;

/*
 *	@brief	ボタン状態によって描画内容を変えるインターフェース
 */
class IButtonStateRenderer {
public:
    /*
     *  @brief  デストラクタ
     */
    virtual ~IButtonStateRenderer() {}

public:
    /*
     *  @brief      描画処理
     *  @param[in]  const Rect& rect
     *  @param[in]  int buttonHandle
     *  @param[in]  const std::string& name
     */
    virtual void Draw(const Rect& rect,
                      int buttonHandle,
                      const std::string& name) = 0;
};

#endif // !_IBUTTON_STATE_RENDERER_H_