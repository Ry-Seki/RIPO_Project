/*
 *	ResistTimeComponent.h
 *	@author Riku
 */

#ifndef _RESISTTIMECOMPONENT_H_
#define _RESISTTIMECOMPONENT_H_

#include "Component.h"

class ResistTimeComponent : public Component {
private:
	float resistTime = 0.0f;		// 現在のレジスト値
	float maxResistTime = 0.0f;	// 最大のレジスト値
	float resistDownSpeed = 0.0f;	// レジスト値の減少スピード

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override {
		DownResist(resistDownSpeed * deltaTime);
	}

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setResistTime, float setDownSpeed) {
		resistTime = setResistTime;
		maxResistTime = setResistTime;
		resistDownSpeed = setDownSpeed;
	}

	/*
	 *	レジスト減少
	 */
	void DownResist(float downValue) {
		if (resistTime - downValue <= 0) {
			resistTime = 0;
		}
		else {
			resistTime - downValue;
		}
	}

public:
	/*
	 *	レジストの取得
	 */
	inline float GetResistTime() const { return resistTime; }
};
// 別名定義
using ResistTimeComponentPtr = std::shared_ptr<ResistTimeComponent>;

#endif // !_RESISTTIMECOMPONENT_H_
