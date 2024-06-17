#include <Resource/Material.h>
#include <Graphics/VertexShader.h>
#include <Graphics/PixelShader.h>
#include <Graphics/ConstantBuffer.h>
#include <Resource/ResourceManager.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <Resource/Texture.h>

Material::Material(const wchar_t* path, ResourceManager* manager) : Resource(path, manager)
{
	auto rsys = manager->getGame()->getGraphicsEngine()->getRenderSystem();

	m_vertex_shader = rsys->createVertexShader(path, "vsmain");
	if (!m_vertex_shader) DX3DError("Material not created successfully");
	
	m_pixel_shader = rsys->createPixelShader(path, "psmain");
	if (!m_pixel_shader)
	{
		DX3DError("Material not created successfully");
	}
	else 
		DX3DInfo("Material created successfully");

}

Material::Material(const MaterialPtr& material, ResourceManager* manager) : Resource(L"", manager)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}

void Material::addTexture(const TexturePtr& texture)
{
	m_vec_textures.push_back(texture->m_texture);
}

void Material::addTexture(const Texture2DPtr& texture)
{
	m_vec_textures.push_back(texture);
}

void Material::removeTexture(ui32 index)
{
	if (index >= this->m_vec_textures.size()) return;
	m_vec_textures.erase(m_vec_textures.begin() + index);
}

void Material::setData(void* data, ui32 size)
{
	auto rsys = m_manager->getGame()->getGraphicsEngine()->getRenderSystem();

	if (!m_constant_buffer)
		m_constant_buffer = rsys->createConstantBuffer(data, size);
	else
		m_constant_buffer->update(rsys->getImmediateDeviceContext(), data);
}

void Material::setMetallic(const float metallic)
{
	m_metallic = metallic;
}

void Material::setRoughness(const float roughness)
{
	m_roughness = roughness;
}

void Material::setReflectance(const float reflectance)
{
	m_reflectance = reflectance;
}

void Material::setCullMode(const CullMode& mode)
{
	m_cull_mode = mode;
}

CullMode Material::getCullMode()
{
	return m_cull_mode;
}