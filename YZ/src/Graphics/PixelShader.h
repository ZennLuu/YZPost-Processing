#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <Prerequisites.h>

class PixelShader
{
public:
	PixelShader(const wchar_t* full_path, const char* entryPoint, RenderSystem* system);
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
	RenderSystem* m_system = nullptr;
private:
	friend class RenderSystem;
	friend class DeviceContext;
};