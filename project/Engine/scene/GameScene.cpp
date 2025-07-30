#include "GameScene.h"

void GameScene::Initialize() {

	ModelManager::GetInstance()->LoadModel("playerHead");
	ModelManager::GetInstance()->LoadModel("terrain");
	ModelManager::GetInstance()->LoadModel("enemy");

 	testClass = new TestClass();
 	testClass->Init();

	spriteUI = new Sprite();
	spriteUI->Initialize("uvChecker.png");

	objectG = new Object3d();
	objectG->Initialize();
	objectG->SetModelFile("terrain");

	wt.Initialize();
	wt.translation_.y = -1.0f;
	wt.scale_ = { 2,2,2 };
}

void GameScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_0)) {
		OutputDebugStringA("Hit 0\n");
	}

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		OutputDebugStringA("Hit 1\n");
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = Title;
	}

	testClass->Update();
	
	bool onLight = false;

	objectG->LightSwitch(onLight);
	//床
	objectG->Update();
	wt.UpdateMatrix();

	
	spriteUI->SetSize(Vector2(128, 128));
	spriteUI->Update();

}

void GameScene::Draw() {

	//スプライト描画処理(背景用)
	SpriteCommon::GetInstance()->Command();


	//モデル描画処理
	Object3dCommon::GetInstance()->Command();

	objectG->Draw(wt);
	testClass->Draw();

	//パーティクル描画処理
	ParticleCommon::GetInstance()->Command();

	//particle->Draw();
	//particle2->Draw();

	//スプライト描画処理(UI用)
	SpriteCommon::GetInstance()->Command();

	spriteUI->Draw();
}
void GameScene::Finalize() {
	
	delete testClass;

	delete spriteUI;
	delete objectG;
}
