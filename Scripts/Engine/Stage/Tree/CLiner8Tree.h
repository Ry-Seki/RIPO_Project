/*
 *	@file	CLiner8Tree.h
 *  @author oorui
 */

#ifndef _CLINER8TREE_H_
#define _CLINER8TREE_H_

#include "../../VecMath.h"
#include <vector>
#include <memory>
#include <list>
#include <cstdint>

 /*
  *	分岐登録オブジェクト
  */
template<class T>
class Object_For_Tree;

template<class T>
class CCell;

/*
 *	分岐登録オブジェクト
 */
template<class T>
class Object_For_Tree {
public:
	CCell<T>* pCell = nullptr;		// 登録空間
	T* pObject = nullptr;			// 判定対象オブジェクト
	Object_For_Tree* next = nullptr;	// 次のObject_For_Tree構造体
	Object_For_Tree* prev = nullptr;	// 前のObject_For_Tree構造体

public:
	/*
	 *	コンストラクタ
	 */
	Object_For_Tree() = default;

	/*
	 *	デストラクタ
	 */
	virtual ~Object_For_Tree() {}

public:

	/*
	 *	自らリストから外れる
	 */
	bool Remove() {
		if (!pCell) return false; // すでに離脱している

		pCell->OnRemove(this); // セル側に通知

		// 前後リンクを接続し直す
		if (prev) prev->next = next;
		if (next) next->prev = prev;

		prev = nullptr;
		next = nullptr;
		pCell = nullptr;
		return true;
	}

	/*
	 *	空間を登録する
	 */
	void RegistCell(CCell<T>* cell) {
		pCell = cell;
	}
};

/*
 *	空間クラス
 */
template<class T>
class CCell {
protected:
	Object_For_Tree<T>* head = nullptr; // セル内先頭ノード

public:
	/*
	 *	コンストラクタ
	 */
	CCell() = default;

	/*
	 *	デストラクタ
	 */
	virtual ~CCell() {
		// 全ノードのリンクを解除（所有権は持たない）
		while (head) {
			auto* next = head->next;
			head->prev = nullptr;
			head->next = nullptr;
			head->pCell = nullptr;
			head = next;
		}
	}

	/*
	 *	OFTをプッシュ
	 */
	bool Push(Object_For_Tree<T>* node) {
		if (!node) return false;
		if (node->pCell == this) return false; // 二重登録防止

		node->next = head;
		node->prev = nullptr;

		if (head) head->prev = node;
		head = node;

		node->RegistCell(this);
		return true;
	}

	Object_For_Tree<T>* GetFirstObj() {
		return head;
	}

	bool OnRemove(Object_For_Tree<T>* node) {
		if (node == head) head = node->next;
		return true;
	}
};

/*
 *	線形八分木空間分割クラス
 */
template<class T>
class CLiner8TreeManager {
protected:
	static constexpr int _MAX_LEVEL = 7;

	std::vector<std::unique_ptr<CCell<T>>> m_cells;
	unsigned int m_iPow[_MAX_LEVEL + 1]{};

	Vector3 m_RgnMin{};
	Vector3 m_RgnMax{};
	Vector3 m_Unit{};
	uint32_t m_dwCellNum = 0;
	unsigned int m_uiLevel = 0;

public:
	/*
	 *	コンストラクタ
	 */
	CLiner8TreeManager() = default;

	/*
	 *	デストラクタ
	 */
	virtual ~CLiner8TreeManager() = default; // unique_ptrに任せる

	/*
	 *	線形八分木配列を構築する
	 */
	bool Init(unsigned int level, const Vector3& min, const Vector3& max) {
		if (level >= _MAX_LEVEL) return false;

		m_uiLevel = level;

		// 各レベルの8^nを計算
		m_iPow[0] = 1;
		for (int i = 1; i <= _MAX_LEVEL; ++i)
			m_iPow[i] = m_iPow[i - 1] * 8;

		m_dwCellNum = (m_iPow[level + 1] - 1) / 7;

		m_cells.clear();
		m_cells.resize(m_dwCellNum);

		m_RgnMin = min;
		m_RgnMax = max;
		m_Unit = (m_RgnMax - m_RgnMin) / float(1 << level);

		return true;
	}

	bool Regist(Vector3* min, Vector3* max, Object_For_Tree<T>* obj) {
		uint32_t elem = GetMortonNumber(*min, *max);
		if (elem >= m_dwCellNum) return false;

		if (!m_cells[elem])
			CreateNewCell(elem);

		return m_cells[elem]->Push(obj);
	}

	uint32_t GetAllCollisionList(std::vector<T*>& ColVect) {
		ColVect.clear();
		if (!m_cells[0]) return 0;

		std::list<T*> ColStack;
		GetCollisionList(0, ColVect, ColStack);
		return (uint32_t)ColVect.size();
	}

protected:

	bool GetCollisionList(uint32_t elem,
		std::vector<T*>& ColVect,
		std::list<T*>& ColStack) {
		auto* node = m_cells[elem]->GetFirstObj();

		// 同一セル内衝突
		while (node) {
			auto* node2 = node->next;
			while (node2) {
				ColVect.push_back(node->pObject);
				ColVect.push_back(node2->pObject);
				node2 = node2->next;
			}
			for (auto* s : ColStack) {
				ColVect.push_back(node->pObject);
				ColVect.push_back(s);
			}
			node = node->next;
		}

		bool childFlag = false;
		uint32_t objNum = 0;

		for (int i = 0; i < 8; ++i) {
			uint32_t nextElem = elem * 8 + 1 + i;
			if (nextElem < m_dwCellNum && m_cells[nextElem]) {

				if (!childFlag) {
					auto* n = m_cells[elem]->GetFirstObj();
					while (n) {
						ColStack.push_back(n->pObject);
						++objNum;
						n = n->next;
					}
				}

				childFlag = true;
				GetCollisionList(nextElem, ColVect, ColStack);
			}
		}

		if (childFlag) {
			for (uint32_t i = 0; i < objNum; ++i)
				ColStack.pop_back();
		}

		return true;
	}

	bool CreateNewCell(uint32_t index) {
		while (!m_cells[index]) {
			m_cells[index] = std::make_unique<CCell<T>>();
			if (index == 0) break;
			index = (index - 1) >> 3;
		}
		return true;
	}

	uint32_t BitSeparateFor3D(uint32_t n) {
		n = (n | n << 8) & 0x0000f00f;
		n = (n | n << 4) & 0x000c30c3;
		n = (n | n << 2) & 0x00249249;
		return n;
	}

	uint32_t Get3DMortonNumber(uint32_t x, uint32_t y, uint32_t z) {
		return BitSeparateFor3D(x)
			| (BitSeparateFor3D(y) << 1)
			| (BitSeparateFor3D(z) << 2);
	}

	uint32_t GetPointElem(const Vector3& p) {
		return Get3DMortonNumber(
			uint32_t((p.x - m_RgnMin.x) / m_Unit.x),
			uint32_t((p.y - m_RgnMin.y) / m_Unit.y),
			uint32_t((p.z - m_RgnMin.z) / m_Unit.z)
		);
	}

	uint32_t GetMortonNumber(const Vector3& Min, const Vector3& Max) {
		uint32_t LT = GetPointElem(Min);
		uint32_t RB = GetPointElem(Max);

		uint32_t Def = RB ^ LT;

		unsigned int HiLevel = 1;
		for (unsigned int i = 0; i < m_uiLevel; ++i) {
			uint32_t check = (Def >> (i * 3)) & 0x7;
			if (check != 0)
				HiLevel = i + 1;
		}

		uint32_t spaceNum = RB >> (HiLevel * 3);
		uint32_t addNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
		spaceNum += addNum;

		if (spaceNum >= m_dwCellNum)
			return 0xffffffff;

		return spaceNum;
	}
};

#endif