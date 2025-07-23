#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include "MyMath.h"


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
	std::vector<ObjectData> objects;
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

};
