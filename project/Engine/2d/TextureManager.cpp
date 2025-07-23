#include "TextureManager.h"

using namespace StringUtility;

TextureManager* TextureManager::instance = nullptr;

uint32_t TextureManager::kSRVIndexTop = 1;


TextureManager* TextureManager::GetInstance() {
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) {
	dxCommon_ = dxCommon;
	this->srvManager = srvManager;
	textureDatas.reserve(SrvManager::kMaxSRVCount);
}


void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void TextureManager::LoadTexture(const std::string& filePath) {

	//読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		return;
	}

	assert(srvManager->Max());

	//テクスチャファイル // byte関連
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップ　//拡大縮小で使う
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));
	
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	
	//最後尾を取得
	TextureData& textureData = textureDatas[filePath];

	textureData.metadata = metadata;
	//textureData.resource = dxCommon_->GetRenderTexture();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);
	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	textureData.srvIndex = srvManager->Allocate();
	textureData.srvHandleCPU = srvManager->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager->GetGPUDescriptorHandle(textureData.srvIndex);

	srvManager->CreateSRVforTexture2D(textureData.srvIndex,textureData.resource.Get(), metadata.format, UINT(metadata.mipLevels));
}

uint32_t TextureManager::GetSrvIndex(const std::string filePath) {
	assert(srvManager->Max());

	TextureData& textureData = textureDatas[filePath];
	return textureData.srvIndex;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string filePath) {
	assert(srvManager->Max());

	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string filePath) {
	assert(srvManager->Max());

	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;
}
