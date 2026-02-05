/*
 *	@file	EffectComponent.h
 *  @author oorui
 */

#ifndef _EFFECT_COMPONENT_H_
#define _EFFECT_COMPONENT_H_
#include"Component.h"
#include "../VecMath.h"
 /*
  *	エフェクトオブジェクト
  * エフェクトの再生などを行う用
  */
class EffectComponent : public Component {
private:
	int resourceHandle;		// エフェクト本体のハンドル
	int playingHandle;		// 再生中のエフェクトハンドル
	bool isLoop;			// ループするか否か
	Vector3 position;		// エフェクトのポジション
	bool isVisible;			// 表示非表示

public:
	/*
	 *	コンストラクタ
	 *  @param		resourcehandle	再生するエフェクトの本体
	 */
	EffectComponent(int& _resourceHandle, bool isLoop);
	~EffectComponent() = default;

public:

	/*
	 *	エフェクト再生
	 */
	void EffectRenderer();

	/*
	 *	エフェクト停止
	 */
	void EffectAllStop();

	/*
	 *	指定エフェクト停止
	 */
	void EffectStop();

	/*
	 * 描画処理
	 */
	void Render();

public:
	/*
	 *	座標の変更
	 *  @param	Vector3 setValue
	 */
	void SetPosition(Vector3 setValue) { position = setValue; }

	/*
	 *　表示非表示の切り替え
	 *  @param	bool setValue
	 */
	void SetVisible(bool setValue) { isVisible = setValue; }

	/*
	 *	@fucntion	IsVisble
	 *	@brief		表示フラグの取得
	 *	@return		bool
	 */
	inline bool IsVisile() const { return isVisible; }

	/*
	 *
	 */
	void SetPlayingHandle(int setPlayingHandle) { playingHandle = setPlayingHandle; }

};

#endif // !_EFFECT_COMPONENT_H_
