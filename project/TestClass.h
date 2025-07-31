#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include "MyMath.h"
#include "Player.h"
#include "Enemy.h"

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
		AABB colliderAABB;
	};
	//敵の生成場所
	struct EnemySpawnData {
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		AABB colliderAABB;
	};
	//カメラの初期位置
	struct CameraInitData {
		Vector3 translation;
		Vector3 rotation;
	};
	////コライダー
	//struct ColliderData {
	//	AABB colliderAABB;
	//};

	std::vector<ObjectData> objects;
	std::vector<PlayerSpawnData> players;
	std::vector<EnemySpawnData> spawnEnemies;
	CameraInitData cameraInit;
	//std::vector<ColliderData> colliders;
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

	Camera* camera = nullptr;
	Vector3 cameraRotate;
	Vector3 cameraTranslate;

	LevelData* levelData;
	std::vector<WorldTransform*> worldTransforms;
	std::vector<Object3d*> objects;

	Player* player;

	std::vector<Enemy*> enemies;
};
