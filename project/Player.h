#pragma once
#include "MyMath.h"
#include "Object3d.h"


class Player {
public:
	Player();
	~Player();
	void Initialize();
	void Update();
	void Draw();

	void SetTranslate(Vector3 translate) { worldTransform.translation_ = translate; }
	void SetRotate(Vector3 rotate) { worldTransform.rotation_ = rotate; }

	void SetAABB(AABB aabb) { playerAABB = aabb; }
	AABB GetAABB();

	void SetModelFile(std::string filename);

private:
	Object3d* object;
	WorldTransform worldTransform;
	AABB playerAABB;
};