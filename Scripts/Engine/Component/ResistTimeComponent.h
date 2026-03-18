/*
 *	ResistTimeComponent.h
 *	@author Riku
 */

#ifndef _RESISTTIMECOMPONENT_H_
#define _RESISTTIMECOMPONENT_H_

#include "Component.h"

class ResistTimeComponent : public Component {
private:
	float resistTime;		// 現在のレジスト値
	float maxResistTime;	// 最大のレジスト値

public:
	/*
	 *	更新処理
	 */
	void Update(float deltaTime) override;

public:
	/*
	 *	セットアップ
	 */
	void Setup(float setValue) {
		resistTime = setValue;
		maxResistTime = setValue;
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

#endif // !_RESISTTIMECOMPONENT_H_
