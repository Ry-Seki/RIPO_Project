#include "StageCollision.h"
#include "../../Component/CapsuleCollider.h"
#include "../../Component/GravityComponent.h"
#include "../../Component/Character/CharacterUtility.h"

using namespace CharacterUtility;

void StageCollision::CollisionUpdate(
	int stageModelHandle,
	GameObject* other,
	Vector3 moveVec
) {
	// 無効な状態なら処理しない
	if (!other || stageModelHandle < 0) return;

	// 移動前後の座標を計算
	Vector3 nowPos = other->position;
	Vector3 prevPos = nowPos - moveVec;

	// 衝突ポリゴン取得
	auto hitDim = SetupCollision(stageModelHandle, other, moveVec);
	if (!hitDim || hitDim->HitNum == 0) return;

	// 壁・床に分類
	std::vector<MV1_COLL_RESULT_POLY*> walls, floors;
	ClassifyPolygons(*hitDim, walls, floors);

	// 壁処理
	ProcessWallCollision(nowPos, prevPos, walls, other);

	// 床処理
	ProcessFloorCollision(nowPos, floors, other);

	// 結果反映
	other->position = nowPos;

	MV1CollResultPolyDimTerminate(*hitDim);
}

std::unique_ptr<MV1_COLL_RESULT_POLY_DIM>
StageCollision::SetupCollision(
	int stageModelHandle,
	GameObject* other,
	Vector3 moveVec
) {
	// カプセル取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return nullptr;

	// カプセル中心
	Vector3 center =
		(other->position + capsule->capsule.segment.startPoint +
			other->position + capsule->capsule.segment.endPoint) * 0.5f;

	// 半径調整
	float radius = capsule->capsule.radius + Magnitude(moveVec);

	// 球判定で取得
	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	*hitDim = MV1CollCheck_Sphere(
		stageModelHandle,
		-1,
		ToVECTOR(center),
		radius
	);

	return hitDim;
}

void StageCollision::ClassifyPolygons(
	const MV1_COLL_RESULT_POLY_DIM& hitDim,
	std::vector<MV1_COLL_RESULT_POLY*>& walls,
	std::vector<MV1_COLL_RESULT_POLY*>& floors
) {
	for (int i = 0; i < hitDim.HitNum; i++) {
		const auto& poly = hitDim.Dim[i];
		bool isWall = (poly.Normal.y < POLYGON_HEIGHT && poly.Normal.y > -POLYGON_HEIGHT);

		if (isWall) {
			walls.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}
		else {
			floors.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}
	}
}

void StageCollision::ProcessWallCollision(
	Vector3& nowPos,
	const Vector3&,
	const std::vector<MV1_COLL_RESULT_POLY*>& walls,
	GameObject* other
) {
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return;

	for (auto* poly : walls) {
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);

		Vector3 capCenter = nowPos;
		Vector3 nearest = Nearest(capCenter, p0, p1, p2);

		Vector3 diff = capCenter - nearest;
		float dist = Magnitude(diff);

		float penetrate = capsule->capsule.radius - dist;
		if (penetrate > 0.0f) {
			nowPos += Normalized(diff) * penetrate;
		}
	}
}

void StageCollision::ProcessFloorCollision(
	Vector3& nowPos,
	const std::vector<MV1_COLL_RESULT_POLY*>& floors,
	GameObject* other
) {
	auto gravity = other->GetComponent<GravityComponent>();
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!gravity || !capsule) return;

	if (floors.empty()) {
		gravity->SetGroundingFrag(false);
		return;
	}

	float maxY = -FLT_MAX;

	for (auto* poly : floors) {
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);

		Vector3 nearest = Nearest(nowPos, p0, p1, p2);
		//maxY = std::max(maxY, nearest.y);
	}

	nowPos.y = maxY - capsule->capsule.segment.startPoint.y;
	gravity->SetGroundingFrag(true);
}
