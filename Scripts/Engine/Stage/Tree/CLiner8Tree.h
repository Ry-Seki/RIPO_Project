/*
 *	@file	CLiner8Tree.h
 *  @author oorui
 */

 // 参考プロジェクト
 // ○×（まるぺけ）つくろーどっとコム様
 // http://marupeke296.com/COL_3D_No15_Octree.html

#ifndef _CLINER8TREE_H_
#define _CLINER8TREE_H_

#include "../../VecMath.h"
#include <vector>
#include <memory>
#include <list>
#include <cstdint>

 // 前方宣言
template<class T>
class Object_For_Tree;
template<class T>
class CCell;

/*
 *	分木登録オブジェクト
 *	@tips	OFT は Object_For_Treeの略なので特に意味はないです。
 */
template<class T>
class Object_For_Tree {
public:
	CCell<T>* pCell = nullptr;			// 登録空間
	T* pObject = nullptr;				// 判定対象オブジェクト
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
		// すでに逸脱している場合は処理終了
		if (!pCell) return false;
		// セル側に自身を通知
		pCell->OnRemove(this);

		// 逸脱処理
		// 前後のオブジェクトを結びつける
		if (prev) prev->next = next;
		if (next) next->prev = prev;

		// リストから外す
		prev = nullptr;
		next = nullptr;
		pCell = nullptr;

		return true;
	}

	/*
	 *	空間を登録する
	 *  @param[in]	cell	登録する空間
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
	// OFTへのスマートポインタ
	Object_For_Tree<T>* headObj = nullptr;

public:
	/*
	 *	コンストラクタ
	 */
	CCell() = default;

	/*
	 *	デストラクタ
	 */
	virtual ~CCell() {
		// 全ノードのリンクを解除
		while (headObj) {
			auto* next = headObj->next;
			headObj->prev = nullptr;
			headObj->next = nullptr;
			headObj->pCell = nullptr;
			headObj = next;
		}
	}

	/*
	 *	セル空間にオブジェクトを登録する
	 *  @param[in]	node	分木登録オブジェクト
	 */
	bool Push(Object_For_Tree<T>* node) {
		// 二重登録防止
		if (!node) return false;
		if (node->pCell == this) return false;

		// 先頭オブジェクトに分木登録オブジェクトを登録
		node->next = headObj;
		// 直前のオブジェクトをnullにする
		node->prev = nullptr;

		// 既存の先頭オブジェクトがあれば、前ポインタを切り替える
		if (headObj) {
			headObj->prev = node;
		}
		// 先頭オブジェクトを更新
		headObj = node;

		// セル情報を登録
		node->RegistCell(this);
		return true;
	}

	/*
	 *	セルに登録されている先頭オブジェクトを返す
	 *  @return headObj		セルに登録されている先頭オブジェクト
	 *  @tips	セル内のオブジェクトを順に走査するために使用
	 */
	Object_For_Tree<T>* GetFirstObj() {
		return headObj;
	}

	/*
	 *	先頭オブジェクトを次の空間に登録する
	 *  @param[in]	空間
	 */
	bool OnRemove(Object_For_Tree<T>* node) {
		// 次のオブジェクトに登録
		if (node == headObj) headObj = node->next;
		return true;
	}
};

/*
 *	線形八分木空間分割クラス
 */
template<class T>
class CLiner8TreeManager {
protected:
	static constexpr int _MAX_LEVEL = 7;			//	最大分割レベル

	std::vector<std::unique_ptr<CCell<T>>> m_cells;	// 空間の配列
	unsigned int m_iPow[_MAX_LEVEL + 1]{};			// 8のべき乗数値配列

	Vector3 m_RgnMin{};				// 空間全体の最小座標
	Vector3 m_RgnMax{};				// 空間全体の最大座標
	Vector3 m_Unit{};				// 最小セルの一つ分の大きさ
	uint32_t m_dwCellNum = 0;		// セルの総数
	unsigned int m_uiLevel = 0;		// 現在の分割レベル

public:
	/*
	 *	コンストラクタ
	 */
	CLiner8TreeManager() = default;

	/*
	 *	デストラクタ
	 */
	virtual ~CLiner8TreeManager() = default;

	/*
	 *	線形八分木配列を構築する
	 *  @param[in]	level	分割レベル
	 *  @param[in]	min		// 最小エリア
	 *  @param[in]	max		// 最大エリア
	 */
	bool Init(unsigned int level, const Vector3& min, const Vector3& max) {
		// 設定された分割レベルが上限を超えていたら抜ける
		if (level >= _MAX_LEVEL) return false;
		// 分割レベルの保存
		m_uiLevel = level;

		// 各レベルでの空間数を算出
		m_iPow[0] = 1;
		for (int i = 1; i <= _MAX_LEVEL; ++i)
			m_iPow[i] = m_iPow[i - 1] * 8;

		// セルの総数を計算
		m_dwCellNum = (m_iPow[level + 1] - 1) / 7;

		// 空間をクリア
		m_cells.clear();
		// 空間の数を変換
		m_cells.resize(m_dwCellNum);

		// 領域を登録
		m_RgnMin = min;
		m_RgnMax = max;
		m_Unit = (m_RgnMax - m_RgnMin) / float(1 << level);

		return true;
	}

	/*
	 *	オブジェクトを登録する
	 *  @param[in]		min		最小座標
	 *  @param[in]		max		最大座標
	 *  @param[in]		obj		空間オブジェクト
	 *  @return[out]	Object_For_Tree		登録したオブジェクトを返す
	 */
	bool Regist(Vector3* min, Vector3* max, Object_For_Tree<T>* obj) {
		// オブジェクトの境界範囲から登録モートン番号を算出
		uint32_t elem = GetMortonNumber(*min, *max);
		if (elem >= m_dwCellNum) return false;

		// 空間がない場合は新規に作成する
		if (!m_cells[elem])
			CreateNewCell(elem);
		return m_cells[elem]->Push(obj);
	}

	/*
	 *	衝突判定リストを作成する
	 *  @param[in]	ColVect		衝突するオブジェクトのリスト
	 */
	uint32_t GetAllCollisionList(std::vector<T*>& ColVect) {
		// リストを初期化
		ColVect.clear();
		// 空間が存在していなければ抜ける
		if (!m_cells[0]) return 0;

		// ルート空間を処理
		std::list<T*> ColStack;
		GetCollisionList(0, ColVect, ColStack);
		return (uint32_t)ColVect.size();
	}

protected:

	/*
	 *	空間内で衝突リストを作成する
	 *  @param[in]	elem		モートン番号
	 *  @param[in]	ColVect		衝突するオブジェクトのリスト
	 *  @param[in]	ColStack	親に存在するオブジェクトのスタック
	 */
	bool GetCollisionList(uint32_t elem,
		std::vector<T*>& ColVect,
		std::list<T*>& ColStack) {
		// 現在のセルの先頭オブジェクトを取得
		auto* node = m_cells[elem]->GetFirstObj();

		// 同一セル内衝突
		while (node) {
			auto* node2 = node->next;
			// 同じセルにいる全オブジェクト同士の組み合わせ
			while (node2) {
				ColVect.push_back(node->pObject);
				ColVect.push_back(node2->pObject);
				node2 = node2->next;
			}
			// 親セルにいるオブジェクトとの衝突
			for (auto* s : ColStack) {
				ColVect.push_back(node->pObject);
				ColVect.push_back(s);
			}
			node = node->next;
		}

		bool childFlag = false;
		uint32_t objNum = 0;

		// 8分木の子ノードを走査
		for (int i = 0; i < 8; ++i) {
			// 線形8分木の子インデックス計算
			uint32_t nextElem = elem * 8 + 1 + i;
			// 子がある場合、現在セルをスタックに積む
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
				// レベル優先の探索を行う
				GetCollisionList(nextElem, ColVect, ColStack);
			}
		}
		// 子の走査が終わったら、追加した分をスタックに戻す
		if (childFlag) {
			for (uint32_t i = 0; i < objNum; ++i)
				ColStack.pop_back();
		}

		return true;
	}

	/*
	 *	セルの生成を行う
	 *  @param[in]	index	セル数
	 */
	bool CreateNewCell(uint32_t index) {
		// 指定セルが存在しなければ生成
		while (!m_cells[index]) {
			m_cells[index] = std::make_unique<CCell<T>>();
			if (index == 0) break;
			index = (index - 1) >> 3;
		}
		return true;
	}

	/*
	 *	ビット分割関数
	 *  @tips	モートン番号算出のために必要
	 */
	uint32_t BitSeparateFor3D(uint32_t n) {
		n = (n | n << 8) & 0x0000f00f;
		n = (n | n << 4) & 0x000c30c3;
		n = (n | n << 2) & 0x00249249;
		return n;
	}

	/*
	 *	3Dモートン空間番号算出関数
	 */
	uint32_t Get3DMortonNumber(uint32_t x, uint32_t y, uint32_t z) {
		// x,y,zのビットを交互に並べた番号を作る
		return BitSeparateFor3D(x)
			| (BitSeparateFor3D(y) << 1)
			| (BitSeparateFor3D(z) << 2);
	}

	/*
	 *	座標から、線形8分木要素番号変換関数
	 *  @param[in]	座標
	 */
	uint32_t GetPointElem(const Vector3& p) {
		// 座標がどの最小セルに属するかモートン番号で返す
		return Get3DMortonNumber(
			uint32_t((p.x - m_RgnMin.x) / m_Unit.x),
			uint32_t((p.y - m_RgnMin.y) / m_Unit.y),
			uint32_t((p.z - m_RgnMin.z) / m_Unit.z)
		);
	}

	/*
	 *	座標から、空間番号を算出
	 *  @param[in]		最小座標
	 *  @param[in]		最大座標
	 */
	uint32_t GetMortonNumber(const Vector3& Min, const Vector3& Max) {
		// 左下と右下のモートン番号を算出
		uint32_t LT = GetPointElem(Min);
		uint32_t RB = GetPointElem(Max);

		// XOR演算子を使用し、
		// どのビットが違うか調べる
		uint32_t Def = RB ^ LT;

		unsigned int HiLevel = 1;
		// 一番上のレベルを調べる
		for (unsigned int i = 0; i < m_uiLevel; ++i) {
			uint32_t check = (Def >> (i * 3)) & 0x7;
			if (check != 0)
				HiLevel = i + 1;
		}
		// そのレベルのセル番号を取得
		uint32_t spaceNum = RB >> (HiLevel * 3);
		// 線形8分木のインデックス補正
		uint32_t addNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
		spaceNum += addNum;

		// 範囲チェック
		if (spaceNum >= m_dwCellNum)
			return 0xffffffff;

		return spaceNum;
	}
public:
	void GetObjectsInAABB(
		const Vector3& min,
		const Vector3& max,
		std::vector<T*>& result) {
		result.clear();

		for (auto& cell : m_cells) {
			if (!cell) continue;

			auto* node = cell->GetFirstObj();
			while (node) {
				T* obj = node->pObject;

				if (!(obj->max.x < min.x || obj->min.x > max.x ||
					obj->max.y < min.y || obj->min.y > max.y ||
					obj->max.z < min.z || obj->min.z > max.z)) {
					result.push_back(obj);
				}

				node = node->next;
			}
		}
	}
};

#endif	_CLINER8TREE_H_