#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include "MyMath.h"
#include "Player.h"


class TestClass {
public:

//LevelData型
struct LevelData {
	struct ObjectData {
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
	};	
	//プレイヤーのスポーン場所
	struct PlayerSpawnData {
		Vector3 translation;
		Vector3 rotation;
	};
	std::vector<ObjectData> objects;
	std::vector<PlayerSpawnData> players;
};

	TestClass();
	~TestClass();

	void Init();
	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	Object3d* object_ = nullptr;
	Camera* camera_ = nullptr;
	bool onLight = false;



	LevelData* levelData;
	std::vector<WorldTransform*> worldTransforms;
	std::vector<Object3d*> objects;

	Player* player;
};
