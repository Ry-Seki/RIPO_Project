#include "CharacterManager.h"

void CharacterManager::Initialize(Engine* setEngine){
	engine = setEngine;
}

void CharacterManager::CreatePlayer(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {

}

void CharacterManager::CreateCharacter(
	const std::string& name,
	const Vector3& position,
	const Vector3& rotation) {
	int objectListCount = static_cast<int>(createObjectList.size());
	for (int i = 0; i < objectListCount; i++) {
		if (createObjectList[i] != nullptr) continue;
		engine->Create<GameObject>(name, position, rotation);
	}
}

