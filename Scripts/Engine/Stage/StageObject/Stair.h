/*
 *	@file	Stair.h
 *  @author oorui
 */

#ifndef _STAIR_H_
#define _STAIR_H_

#include "StageObjectBase.h"

class Stair : public StageObjectBase {
private:
	bool isStairMoved = false;		// ŠK‘wˆÚ“®ƒtƒ‰ƒO
	int	stairID = -1;				// ŠK’i‚ÌID
	int nextID = -1;				// Ÿ‚ÌŠK‘w‚ÌID
	int prevID = -1;				// ‚Ğ‚Æ‚Â‘O‚ÌŠK‘w‚ÌID

public:
	Stair() :isStairMoved(false) {};		// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	~Stair() override {};		// ƒfƒXƒgƒ‰ƒNƒ^
public:

	/*
	 *	ŠK‘wˆÚ“®ƒtƒ‰ƒO‚É•ÏX‚ğ‰Á‚¦‚é
	 *  @param	self
	 *  @param	otger
	 */
	void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other)override;

public:

	/*
	 *	ŠK‘wˆÚ“®ƒtƒ‰ƒO‚Ìæ“¾
	 *  @return isStairMoved
	 */
	inline bool GetStairMove()const {
		return isStairMoved;
	}
	// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	void ModelLoad(const int modelHandleBase) override;
	/*
	 *	ŠK’iˆÚ“®‚Ìƒtƒ‰ƒO‚É•ÏX‚ğ‰Á‚¦‚é
	 *  @param	setValue
	 *  @tips	true  = G‚ê‚½
	 *  @tips	false = G‚ê‚Ä‚È‚¢
	 */
	void SetStairMove(bool setValue) { isStairMoved = setValue; }

public:

	/*
	 *	ŠK’i‚ÌID‚Ìæ“¾
	 */
	inline int GetStairID()const { return stairID; };

	/*
	 *	ŠK’i‚ÌID‚Ì•ÏX
	 *  @param	setValue	ID”Ô
	 */
	inline void SetStairID(int setValue) { stairID = setValue; }

	/*
	 *	Ÿ‚ÌŠK‘wID‚Ìæ“¾
	 */
	inline int GetNextID()const { return nextID; }

	/*
	 *	Ÿ‚ÌŠK‘wID‚Ì•ÏX
	 *  @oaram	setValue	ID”Ô
	 */
	inline void SetNextID(int setValue) { nextID = setValue; }

	/*
	 *	‚Ğ‚Æ‚Â‘O‚ÌŠK‘wID‚Ìæ“¾
	 */
	inline int GetPrevID()const { return prevID; }

	/*
	 *	‚Ğ‚Æ‚Â‘O‚ÌŠK‘wID‚Ì•ÏX
	 *  @oaram	setValue	ID”Ô
	 */
	inline void SetPrevID(int setValue) { prevID = setValue; }

};
#endif // !_STAIR_H_

