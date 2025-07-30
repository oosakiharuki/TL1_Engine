#include "Player.h"
#include "Input.h"

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
	//if (Input::GetInstance()->PushKey(DIK_W)) {
	//	worldTransform.translation_.y = 1.0f;
	//}
	//else {
	//	worldTransform.translation_.y = 0.0f;
	//}

	worldTransform.UpdateMatrix();
}

void Player::Draw() {
	object->Draw(worldTransform);
}
