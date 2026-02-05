/*
 *	@file	GaugeBase.h
 *	@author	kuu
 */
#ifndef _GAUGEBASE_H_
#define _GAUGEBASE_H_

/*
 *	ゲージの基底クラス
 */
class GaugeBase {
public:
	/*
	 *	コンストラクタ
	 */
	GaugeBase() = default;
	~GaugeBase() = default;

	virtual void Start() {};
	virtual void Update() {};
	virtual void Execute() {};
};

#endif // !_GAUGEBASE_H_
