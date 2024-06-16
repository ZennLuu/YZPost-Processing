#include <Graphics/RenderSystem.h>
#include <Graphics/SwapChain.h>
#include <Graphics/DeviceContext.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/IndexBuffer.h>
#include <Graphics/ConstantBuffer.h>
#include <Graphics/VertexShader.h>
#include <Graphics/PixelShader.h>
#include <Graphics/Font2D.h>

#include <d3dcompiler.h>
#include <exception>

#include "imgui/imgui_impl_dx11.h"

RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	ui32 num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	ui32 num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	D3D_FEATURE_LEVEL featureLevel = {};

	for (ui32 driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &featureLevel, &m_imm_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		DX3DError("RenderSystem not created successfully");
	}

	m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context.Get(), this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	m_imageBatch = std::make_unique<DirectX::DX11::SpriteBatch>(this->m_imm_context.Get());

	initRasterizerStates();
	compilePrivateShaders();

}

RenderSystem::~RenderSystem()
{
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, ui32 width, ui32 height)
{
	return std::make_shared<SwapChain>(hwnd, width, height, this);
}


DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, ui32 size_vertex, ui32 size_list)
{
	return std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, this);
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, ui32 size_list)
{
	return std::make_shared<IndexBuffer>(list_indices, size_list, this);
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, ui32 size_buffer)
{
	return std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
}

VertexShaderPtr RenderSystem::createVertexShader(const wchar_t* full_path, const char* entryPoint)
{
	return std::make_shared<VertexShader>(full_path, entryPoint, this);
}

PixelShaderPtr RenderSystem::createPixelShader(const wchar_t* full_path, const char* entryPoint)
{
	return std::make_shared<PixelShader>(full_path, entryPoint, this);
}

Texture2DPtr RenderSystem::createTexture(const wchar_t* full_path)
{
	return std::make_shared<Texture2D>(full_path, this);
}

Texture2DPtr RenderSystem::createTexture(const Rect& size, Texture2D::Type type)
{
	return std::make_shared<Texture2D>(size, type, this);
}
Font2DPtr RenderSystem::createFont(const wchar_t* path)
{
	return std::make_shared<Font2D>(path, this);
}

void RenderSystem::setCullMode(const CullMode& mode)
{
	if (mode == CullMode::Front)
		m_imm_context->RSSetState(m_cull_front_state.Get());
	else if (mode == CullMode::Back)
		m_imm_context->RSSetState(m_cull_back_state.Get());
	else if (mode == CullMode::None)
		m_imm_context->RSSetState(m_cull_none_state.Get());
}

void RenderSystem::clearState()
{
	m_imm_context->ClearState();
	m_imm_context->OMSetBlendState(m_alphaBlendState.Get(), 0, 0xffffffff);
}

void RenderSystem::drawImage(const Texture2DPtr& texture, const Rect& size)
{
	m_imageBatch->Begin(DirectX::DX11::SpriteSortMode_Deferred, m_alphaBlendState.Get());
	RECT rc = { size.left,size.top,size.left + size.width,size.top + size.height };
	m_imageBatch->Draw(texture->m_shader_res_view.Get(), rc);
	m_imageBatch->End();
}

void RenderSystem::compilePrivateShaders()
{
	Microsoft::WRL::ComPtr<ID3DBlob>  blob;
	Microsoft::WRL::ComPtr<ID3DBlob>  errorBlob;

	auto meshLayoutCode = R"(
struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent: TANGENT0;
	float3 binormal: BINORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	return output;
}
	)";

	auto codeSize = strlen(meshLayoutCode);

	if (FAILED(D3DCompile(meshLayoutCode, codeSize, "VertexMeshLayoutShader", nullptr, nullptr,
		"vsmain", "vs_5_0", 0, 0, &blob, &errorBlob)))
	{
		DX3DError("VertexMeshLayoutShader not compiled successfully");
	}

	memcpy(m_meshLayoutByteCode, blob->GetBufferPointer(), blob->GetBufferSize());
	m_meshLayoutSize = blob->GetBufferSize();
}

void RenderSystem::initRasterizerStates()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;

	desc.CullMode = D3D11_CULL_FRONT;
	m_d3d_device->CreateRasterizerState(&desc, &m_cull_front_state);
	desc.CullMode = D3D11_CULL_BACK;
	m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);
	desc.CullMode = D3D11_CULL_NONE;
	m_d3d_device->CreateRasterizerState(&desc, &m_cull_none_state);



	D3D11_BLEND_DESC blendDesc = {};

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_d3d_device->CreateBlendState(&blendDesc, &m_alphaBlendState);
	m_imm_context->OMSetBlendState(m_alphaBlendState.Get(), 0, 0xffffffff);
}