#pragma once
#include <Prerequisites.h>

#include <d3d11.h> 
#include <wrl.h>

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, ui32 size_buffer,RenderSystem * system);

	void update(const DeviceContextPtr& context, void* buffer);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	RenderSystem * m_system = nullptr;
private:
	friend class DeviceContext;
};