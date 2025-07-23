#pragma once
#include "DirectXCommon.h"

class PostEffect{
public:

	static PostEffect* GetInstance();
	void Finalize();

	void Initialize(DirectXCommon* dxCommon);
	DirectXCommon* GetDirectXCommon()const { return dxCommon_; }

	void Command();
private:
	//PSO
	void RootSignature();
	void GraphicsPipeline();

	static PostEffect* instance;

	PostEffect() = default;
	~PostEffect() = default;
	PostEffect(PostEffect&) = default;
	PostEffect& operator=(PostEffect&) = default;

	static uint32_t kSRVIndexTop;

	DirectXCommon* dxCommon_;

	//RootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState = nullptr;

	uint32_t srvIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
};