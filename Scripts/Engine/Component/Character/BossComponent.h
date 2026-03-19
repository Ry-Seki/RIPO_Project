/*
 *	@file	BossComponent.h
 *  @author kuu
 */
#ifndef _BOSSCOMPONENT_H_
#define _BOSSCOMPONENT_H_

#include "../Component.h"
#include "BossState.h"
#include "../AnimatorComponent.h"
#include "../ModelRenderer.h"
#include "../../Manager/EnemyDataManager.h"
#include "../../GameEnum.h"
#include "../../Load/Audio/LoadAudio.h"
#include "../../Audio/AudioUtility.h"

class BossComponent : public Component {
private:
	GameObject* boss;
	GameObjectPtr player;
	std::shared_ptr<AnimatorComponent> animator;
	BossState* state;
	struct EnemyDataManager::EnemyStatus status;
	// モデルハンドル
	int modelHandle;
	// ボスのHP
	int HP;
	// ボスのID
	int ID = -1;
	// クールタイム
	float coolTime;
	// ランダム攻撃のクールタイム
	float randomCoolTime;
	// 攻撃衝突判定フラグ
	bool attackIsTriger;
	// ダメージ衝突判定フラグ
	bool damageIsTriger;
	// 行動フラグ
	bool moveFrag;
	// 被ダメフラグ
	bool hitFlag = false;
	// 攻撃中判定フラグ
	bool bossAttackTimeFlag = false;
	// 近距離攻撃範囲判定
	bool closeRangeAttackDistanceFlag = false;
	// 遠距離攻撃範囲判定
	bool longRangeAttackDistanceFlag = false;
	// 定位置
	Vector3 homePosition;
	// 移動量
	Vector3 moveVec;

public:
	/*
	 *	コンストラクタ
	 */
	BossComponent();
	BossComponent(BossState* initState);
	~BossComponent();

public:
	virtual void Start() override;
	/*
	 *	更新処理
	 */
	virtual void Update(float deltaTime) override;

	/*
	 *	衝突が起きたときに呼び出される処理
	 */
	virtual void OnCollision(const std::shared_ptr<Component>& self, const std::shared_ptr<Component>& other) override;

public:
	/*
	 *	IDを知った後に呼び出す初期化処理
	 */
	void SetBossStart(int ID);

public:
	/*
	 *	位置の取得
	 */
	inline Vector3 GetBossPosition() const {
		if (boss == nullptr) return Vector3::zero;
		return boss->position;
	}

	/*
	 *	位置の変更
	 */
	inline void SetBossPosition(Vector3 setValue) {
		boss->position = setValue;
	}

	/*
	 *	角度の取得
	 */
	inline Vector3 GetBossRotation() const { return boss->rotation; }

	/*
	 *	角度の変更
	 */
	inline void SetBossRotation(Vector3 setValue) {
		boss->rotation = setValue;
	}

	/*
	 *	状態の変更
	 *  param[in]	BossState*	setValue	次の状態
	 */
	inline void SetState(BossState* setValue) {
		if (state) {
			state->Execute(*this);
		}
		state = setValue;
		state->Start(boss);
	}

	/*
	 *	定位置の取得
	 */
	inline Vector3 GetHomePosition() const { return homePosition; }

	/*
	 *	HPの取得
	 */
	inline int GetBossHP() const { return HP; }

	/*
	 *	HPの変更
	 */
	inline void SetBossHP(int setValue) { HP -= setValue; }

	/*
	 *	最大HPの取得
	 */
	inline int GetBossMaxHP() const { return status.HP; }

	/*
	 *	攻撃力の取得
	 */
	inline int GetBossAttack() const { return status.attack; }

	/*
	 *	IDの取得
	 */
	inline int GetBossID() const { return status.ID; }

	/*
	 *	討伐金データの取得
	 */
	inline int GetBossDataBounty() const { return status.bounty; }

	/*
	 *	攻撃衝突判定の取得
	 */
	inline bool GetAttackIsTriger() const { return attackIsTriger; }

	/*
	 *	行動フラグの取得
	 */
	inline bool GetMoveFrag() const { return moveFrag; }

	/*
	 *	行動フラグの変更
	 */
	inline void SetMoveFrag(bool setValue) {
		moveFrag = setValue;
	}

	/*
	 *	被ダメフラグの取得
	 */
	inline bool GetHitFlag() const { return hitFlag; }

	/*
	 *	被ダメフラグの変更
	 */
	inline void SetHitFlag(bool setValue) { hitFlag = setValue; }

	/*
	 *	攻撃中フラグの取得
	 */
	inline bool GetBossAttackTimeFlag() const { return bossAttackTimeFlag; }

	/*
	 *	攻撃中フラグの変更
	 */
	inline void SetBossAttackTimeFlag(bool setValue) { bossAttackTimeFlag = setValue; }

	/*
	 *	近距離攻撃範囲判定の取得
	 */
	inline bool GetCloseRangeAttackDistanceFlag() const { return closeRangeAttackDistanceFlag; }

	/*
	 *	近距離攻撃範囲判定の変更
	 */
	inline void SetCloseRangeAttackDistanceFlag(bool setValue) { closeRangeAttackDistanceFlag = setValue; }

	/*
	 *	遠距離攻撃範囲判定の取得
	 */
	inline bool GetLongRangeAttackDistanceFlag() const { return longRangeAttackDistanceFlag; }

	/*
	 *	遠距離攻撃範囲判定の変更
	 */
	inline void SetLongRangeAttackDistanceFlag(bool setValue) { longRangeAttackDistanceFlag = setValue; }

	/*
	 *	ボスから見たプレイヤーの方向
	 */
	inline Vector3 GetBossToPlayerDirection() { return Direction(boss->position, player->position); }
	
	/*
	 *	ランダムクールタイムの取得
	 */
	inline float GetRandomCoolTime() const { return randomCoolTime; }

	/*
	 *	ランダムクールタイムの変更
	 */
	inline void SetRandomCoolTime(float setValue) { randomCoolTime = setValue; }
};

#endif // !_BOSSCOMPONENT_H_