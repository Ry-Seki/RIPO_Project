/*
 *	@file	CharacterBase.h
 */

#ifndef _CHARACTERBASE_H_
#define _CHATACTERBASE_H_

#include "../Component.h"

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
	 *	@function	isDead
	 *	@brief		Ž€–S”»’è
	 *	@return		bool
	 */
	inline bool isDead() const {
		return HP <= 0;
	}
public:
	/*
	 *	@function	GetID
	 *	@brief		ID‚ÌŽæ“¾
	 *	@return		int
	 */
	inline int GetID() const {
		return ID;
	}
	/*
	 *	@function	SetID
	 *	@brief		ID‚Ì•ÏX
	 *	@param[in]	int& setValue;
	 */
	inline void GetID(int& setValue) {
		ID = setValue;
	}
	/*
	 *	@function	GetNameID
	 *	@brief		–¼‘OID‚ÌŽæ“¾
	 *	@return		int
	 */
	inline int GetNameID() const {
		return nameID;
	}
	/*
	 *	@function	SetNameID
	 *	@brief		–¼‘OID‚Ì•ÏX
	 *	@param[in]	int& setValue
	 */
	inline void SetNameID(int& setValue){
		nameID = setValue;
	}
	/*
	 *	@function	GetMaxHP
	 *	@brief		Å‘åHP‚ÌŽæ“¾
	 *	@return		int
	 */
	inline int GetMaxHP() const {
		return maxHP;
	}
	/*
	 *	@function	SetMaxHP
	 *	@brief		Å‘åHP‚Ì•ÏX
	 *	@param[in]	int& setValue
	 */
	inline void GetMaxHP(int& setValue){
		maxHP = setValue;
	}
	/*
	 *	@function	GetHP
	 *	@brief		HP‚ÌŽæ“¾
	 *	@return		int
	 */
	inline int GetHP() const {
		return HP;
	}
	/*
	 *	@function	SetHP
	 *	@brief		HP‚Ì•ÏX
	 *	@param[in]	int& setValue
	 */
	inline int SetHP(int& setValue){
		HP = setValue;
	}
	/*
	 *	@function	GetRawAttack
	 *	@brief		‘f‚ÌUŒ‚—Í‚ÌŽæ“¾
	 *	@return		int
	 */
	inline int GetRawAttack() const {
		return rawAttack;
	}
	/*
	 *	@function	SetRawAttack
	 *	@brief		‘f‚ÌUŒ‚—Í‚Ì•ÏX
	 *	@param[in]	int& setValue
	 */
	inline int GetRawAttack(int& setValue) {
		rawAttack = setValue;
	}
};
// •Ê–¼’è‹`
using CharacterBasePtr = std::shared_ptr<CharacterBase>;
using CharacterBaseList = std::vector<CharacterBasePtr>;

#endif // !_CHARACTERBASE_H_


