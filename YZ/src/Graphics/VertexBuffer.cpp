#include <Graphics/VertexBuffer.h>
#include <Graphics/RenderSystem.h>
#include <exception>

VertexBuffer::VertexBuffer(void* list_vertices, ui32 size_vertex, ui32 size_list, RenderSystem* system) : m_system(system), m_layout(0), m_buffer(0)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("VertexBuffer not created successfully");
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{"NORMAL", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{"TANGENT", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 32,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{"BINORMAL", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA ,0 }
	};

	ui32 size_layout = ARRAYSIZE(layout);

	if (FAILED(m_system->m_d3d_device->CreateInputLayout(layout, size_layout, m_system->m_meshLayoutByteCode, m_system->m_meshLayoutSize, &m_layout)))
	{
		throw std::exception("InputLayout not created successfully");
	}

}

ui32 VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}
