#include <Graphics/ConstantBuffer.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/DeviceContext.h>
#include <exception>

ConstantBuffer::ConstantBuffer(void * buffer, ui32 size_buffer,RenderSystem * system) : m_system(system)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_buffer;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("ConstantBuffer not created successfully");
	}
}

void ConstantBuffer::update(const DeviceContextPtr& context, void * buffer)
{
	context->m_device_context->UpdateSubresource(this->m_buffer.Get(), NULL, NULL, buffer, NULL, NULL);
}


