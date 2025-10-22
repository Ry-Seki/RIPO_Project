/*
 *	@file	CharacterBase.h
 *	@author	Seki
 */

#ifndef _CHARACTERBASE_H_
#define _CHARACTERBASE_H_

#include "../../Component/Component.h"
#include <vector>

/*
 *	キャラクターの基底クラス
 */
class CharacterBase : public Component {
private:
	int ID = -1;
	int masterID = -1;

	int nameID = -1;
	int maxHP = -1;
	int HP = -1;
	int rawAttack = -1;

public:
	/*
	 *	死亡判定
	 *	@return		bool
	 */
	inline bool isDead() const {
		return HP <= 0;
	}
public:
	/*
	 *	IDの取得 
	 *	@return		int
	 */
	inline int GetID() const {
		return ID;
	}
	/*
	 *	IDの変更
	 *	@param[in]	int& setValue;
	 */
	inline void SetID(int& setValue) {
		ID = setValue;
	}
	/*
	 *	名前IDの取得
	 *	@return		int
	 */
	inline int GetNameID() const {
		return nameID;
	}
	/*
	 *	名前IDの変更
	 *	@param[in]	int& setValue
	 */
	inline void SetNameID(int& setValue){
		nameID = setValue;
	}
	/*
	 *	最大HPの取得
	 *	@return		int
	 */
	inline int GetMaxHP() const {
		return maxHP;
	}
	/*
	 *	最大HPの変更
	 *	@param[in]	int& setValue
	 */
	inline void SetMaxHP(int& setValue){
		maxHP = setValue;
	}
	/*
	 *	HPの取得
	 *	@return		int
	 */
	inline int GetHP() const {
		return HP;
	}
	/*
	 *	HPの変更
	 *	@param[in]	int& setValue
	 */
	inline int SetHP(int& setValue){
		HP = setValue;
	}
	/*
	 *	素の攻撃力の取得
	 *	@return		int
	 */
	inline int GetRawAttack() const {
		return rawAttack;
	}
	/*
	 *	素の攻撃力の変更
	 *	@param[in]	int& setValue
	 */
	inline int SetRawAttack(int& setValue) {
		rawAttack = setValue;
	}
};
// 別名定義
using CharacterBasePtr = std::shared_ptr<CharacterBase>;
using CharacterBaseList = std::vector<CharacterBasePtr>;

#endif // !_CHARACTERBASE_H_


