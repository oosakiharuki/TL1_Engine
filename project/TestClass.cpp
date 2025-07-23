#include "TestClass.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"

#include <json.hpp>
#include <fstream>

using namespace MyMath;

TestClass::TestClass(){}

TestClass::~TestClass() {
	delete object_;
	
	for (auto& object : objects) {
		delete object;
	}
	objects.clear();
}

void TestClass::Init() {
	worldTransform_.Initialize();

	object_ = new Object3d();
	object_->Initialize();
	object_->SetModelFile("grass01");
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };	
	
	levelData = new LevelData();

	//json
	//ファイルを選択
	const std::string fullpath = "resource/Levelediter/TL1_01_12.json";

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


}

void TestClass::Update() {
	

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

#endif // _DEBUG

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

}
