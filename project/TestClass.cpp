#include "TestClass.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"

#include <json.hpp>
#include <fstream>

#include "Object3dCommon.h"
#include "ParticleCommon.h"

using namespace MyMath;

TestClass::TestClass(){}

TestClass::~TestClass() {
	delete object_;
	
	for (auto& object : objects) {
		delete object;
	}
	objects.clear();
	delete player;

	for (auto& enemy : enemies) {
		delete enemy;
	}
	enemies.clear();
	delete camera;
}

void TestClass::Init() {
	worldTransform_.Initialize();

	object_ = new Object3d();
	object_->Initialize();
	object_->SetModelFile("terrain");
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };	
	
	levelData = new LevelData();


	camera = new Camera();
	Object3dCommon::GetInstance()->SetDefaultCamera(camera);
	ParticleCommon::GetInstance()->SetDefaultCamera(camera);

	//json
	//ファイルを選択
	const std::string fullpath = "resource/Levelediter/scene.json";

	//ファイルストリーム
	std::ifstream file;

	//読み取れない場合
	file.open(fullpath);

	if (file.fail()) {
		assert(0);
	}

	//Json文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍処理
	file >> deserialized;

	assert(deserialized.is_object());//オブジェクトがあるか
	assert(deserialized.contains("name"));//名前があるか
	assert(deserialized["name"].is_string());//stringであるか
	
	//["name"]文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	
	//正しいレベルデータファイルなのか
	assert(name.compare("scene") == 0);


	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		if (object.contains("disabled")) {
			bool disabled = object["disabled"].get<bool>();//jsonの値を入れる
			if (disabled) {
				//Trueの場合 配置しない
				continue;
			}
		}


		std::string type = object["type"].get<std::string>();

		if (type.compare("MESH") == 0) {
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//BlenderのY軸とZ軸と違うため y = [2],z = [1]
			//移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転
			objectData.rotation.x = (float)transform["rotation"][0];
			objectData.rotation.y = (float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][1];
			//スケール
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];
		}
		else if (type.compare("PlayerSpawn") == 0) {
			//要素追加
			levelData->players.emplace_back(LevelData::PlayerSpawnData{});
			//
			LevelData::PlayerSpawnData& playerSpawnData = levelData->players.back();
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//BlenderのY軸とZ軸と違うため y = [2],z = [1]
			//移動
			playerSpawnData.translation.x = (float)transform["translation"][0];
			playerSpawnData.translation.y = (float)transform["translation"][2];
			playerSpawnData.translation.z = (float)transform["translation"][1];
			//回転
			playerSpawnData.rotation.x = (float)transform["rotation"][0];
			playerSpawnData.rotation.y = (float)transform["rotation"][2];
			playerSpawnData.rotation.z = (float)transform["rotation"][1];

			//コライダー
			nlohmann::json& collider = object["collider"];

			//Vectorに変換
			Vector3 center = { (float)collider["center"][0],(float)collider["center"][2], (float)collider["center"][1]};
			Vector3 size = { (float)collider["size"][0],(float)collider["size"][2], (float)collider["size"][1] };

			//AABBに追加
			playerSpawnData.colliderAABB.min = center - (size / 2.0f);
			playerSpawnData.colliderAABB.max = center + (size / 2.0f);
		}
		else if (type.compare("EnemySpawn") == 0) {
			//要素追加
			levelData->spawnEnemies.emplace_back(LevelData::EnemySpawnData{});
			//
			LevelData::EnemySpawnData& enemySpawnData = levelData->spawnEnemies.back();
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//BlenderのY軸とZ軸と違うため y = [2],z = [1]
			//移動
			enemySpawnData.translation.x = (float)transform["translation"][0];
			enemySpawnData.translation.y = (float)transform["translation"][2];
			enemySpawnData.translation.z = (float)transform["translation"][1];
			//回転
			enemySpawnData.rotation.x = (float)transform["rotation"][0];
			enemySpawnData.rotation.y = (float)transform["rotation"][2];
			enemySpawnData.rotation.z = (float)transform["rotation"][1];

			//enemySpawnData.fileName = transform["name"];

			//コライダー
			nlohmann::json& collider = object["collider"];

			//Vectorに変換
			Vector3 center = { (float)collider["center"][0],(float)collider["center"][2], (float)collider["center"][1] };
			Vector3 size = { (float)collider["size"][0],(float)collider["size"][2], (float)collider["size"][1] };

			//AABBに追加
			enemySpawnData.colliderAABB.min = center - (size / 2.0f);
			enemySpawnData.colliderAABB.max = center + (size / 2.0f);
		}
		else if (type.compare("CAMERA") == 0) {
			//要素追加
			levelData->cameraInit = LevelData::CameraInitData{};
			//
			LevelData::CameraInitData& cameraInitData = levelData->cameraInit;
			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//移動
			cameraInitData.translation.x = (float)transform["translation"][0];
			cameraInitData.translation.y = (float)transform["translation"][2];
			cameraInitData.translation.z = (float)transform["translation"][1];
			//回転
			cameraInitData.rotation.x = (float)transform["rotation"][0] - 90.0f;//blenderだと0度で真下を向くため
			cameraInitData.rotation.y = (float)transform["rotation"][2];
			cameraInitData.rotation.z = (float)transform["rotation"][1];

			cameraRotate = cameraInitData.rotation;
			cameraTranslate = cameraInitData.translation;
		}

		//子ノード
		if (object.contains("children")) {

		}
	}

	for (auto& objectData : levelData->objects) {
		//ファイル名から登録済みのモデル検索
		Object3d* newObject = new Object3d();
		newObject->Initialize();
		newObject->SetModelFile(objectData.fileName);
		objects.push_back(newObject);
	}
		
	for (auto& objectData : levelData->objects) {

		WorldTransform* newObject = new WorldTransform();
		newObject->Initialize();

		newObject->translation_ = objectData.translation;

		newObject->rotation_ = objectData.rotation;

		newObject->scale_ = objectData.scaling;

		worldTransforms.push_back(newObject);
	}

	player = new Player();
	player->Initialize();
	
	//プレイヤー配置データがあるときプレイヤーを配置
	if (!levelData->players.empty()) {
		auto& playerData = levelData->players[0];
		player->SetTranslate(playerData.translation);
		player->SetRotate(playerData.rotation);
		player->SetAABB(playerData.colliderAABB);
	}

	if (!levelData->spawnEnemies.empty()) {
		for (auto& enemyData : levelData->spawnEnemies) {
			Enemy* enemy = new Enemy();
			enemy->Initialize();
			enemy->SetTranslate(enemyData.translation);
			enemy->SetRotate(enemyData.rotation);
			enemy->SetAABB(enemyData.colliderAABB);
			enemies.push_back(enemy);			
		}
	}

	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);

}

void TestClass::Update() {
	
	player->Update();

	for (auto& enemy : enemies) {
		enemy->Update();
		
		if(IsCollisionAABB(player->GetAABB(),enemy->GetAABB())){
			enemy->IsHit();
		}

	}

	camera->Update();



	onLight = true;

#ifdef _DEBUG

	ImGui::Begin("TestModel");

	ImGui::InputFloat3("VertexModel", &worldTransform_.translation_.x);
	ImGui::SliderFloat3("SliderVertexModel", &worldTransform_.translation_.x, -5.0f, 5.0f);

	ImGui::InputFloat3("RotateModel", &worldTransform_.rotation_.x);
	ImGui::SliderFloat3("SliderRotateModel", &worldTransform_.rotation_.x, -10.0f, 10.0f);

	ImGui::InputFloat3("ScaleModel", &worldTransform_.scale_.x);
	ImGui::SliderFloat3("SliderScaleModel", &worldTransform_.scale_.x, 0.5f, 5.0f);

	ImGui::End();

	//ここにテキストを入れられる

	//開発用UIの処理
	//ImGui::ShowDemoWindow();

	ImGui::Begin("camera");
	ImGui::Text("ImGuiText");

	//カメラ
	ImGui::InputFloat3("cameraTranslate", &cameraTranslate.x);
	ImGui::SliderFloat3("cameraTranslateSlider", &cameraTranslate.x, -30.0f, 30.0f);
	
	ImGui::InputFloat3("cameraRotate", &cameraRotate.x);
	ImGui::SliderFloat("cameraRotateX", &cameraRotate.x, -360.0f, 360.0f);
	ImGui::SliderFloat("cameraRotateY", &cameraRotate.y, -360.0f, 360.0f);
	ImGui::SliderFloat("cameraRotateZ", &cameraRotate.z, -360.0f, 360.0f);
	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);

	ImGui::End();
#endif //  USE_IMGUI

	object_->LightSwitch(onLight);
	
	for (WorldTransform* objectData : worldTransforms) {
		objectData->UpdateMatrix();
	}

	worldTransform_.UpdateMatrix();
}


void TestClass::Draw() {

	int i = 0;//順番に
	for (auto& object : objects) {

		object->Draw(*worldTransforms[i]);

		i++;
	}

	//object_->Draw(worldTransform_);
	player->Draw();

	for (auto& enemy : enemies) {
		enemy->Draw();
	}

}
