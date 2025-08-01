#include "Player.h"
#include "Input.h"

using namespace MyMath;

Player::Player(){}

Player::~Player() {
	delete object;
}

void Player::Initialize() {
	worldTransform.Initialize();

	object = new Object3d();
	object->Initialize();
	object->SetModelFile("playerHead");
}

void Player::Update() {
	if (Input::GetInstance()->PushKey(DIK_A)) {
		worldTransform.translation_.x -= 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_D)) {
		worldTransform.translation_.x += 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_W)) {
		worldTransform.translation_.z += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_S)) {
		worldTransform.translation_.z -= 0.1f;
	}

	worldTransform.UpdateMatrix();
}

void Player::Draw() {
	object->Draw(worldTransform);
}

AABB Player::GetAABB() {
	AABB aabb;
	aabb.min = worldTransform.translation_ + playerAABB.min;
	aabb.max = worldTransform.translation_ + playerAABB.max;
	return aabb;
}

void Player::SetModelFile(std::string filename) {

}
