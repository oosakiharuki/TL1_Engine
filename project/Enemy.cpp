#include "Enemy.h"
#include "Input.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	delete object;
}

void Enemy::Initialize() {
	worldTransform.Initialize();

	object = new Object3d();
	object->Initialize();
	object->SetModelFile("enemy");
}

void Enemy::Update() {

	worldTransform.rotation_.y += 0.05f;
	worldTransform.UpdateMatrix();
}

void Enemy::Draw() {
	object->Draw(worldTransform);
}
