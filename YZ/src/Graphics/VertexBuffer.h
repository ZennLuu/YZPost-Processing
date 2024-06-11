#pragma once
#include <Prerequisites.h>

#include <d3d11.h> 
#include <wrl.h>

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, ui32 size_vertex, ui32 size_list,
		RenderSystem * system);
	ui32 getSizeVertexList();
private:
	ui32 m_size_vertex;
	ui32 m_size_list;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	RenderSystem * m_system = nullptr;
private:
	friend class DeviceContext;
};

