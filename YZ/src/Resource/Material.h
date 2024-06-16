#pragma once
#include <Resource/Resource.h>
#include <vector>

class GraphicsEngine;
class Material : public Resource
{
public:
	Material(const wchar_t* path, ResourceManager* manager);
	Material(const MaterialPtr& material, ResourceManager* manager);

	void addTexture(const TexturePtr& texture);
	void addTexture(const Texture2DPtr& texture);
	void removeTexture(ui32 index);

	void setData(void* data, ui32 size);

	void setMetallic(const float metallic);
	void setRoughness(const float roughness);
	void setReflectance(const float reflectance);


	void setCullMode(const CullMode& mode);
	CullMode getCullMode();

private:

	float m_metallic = 0.0;
	float m_roughness = 0.5;
	float m_reflectance = 0.5;

	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<Texture2DPtr> m_vec_textures;
	CullMode m_cull_mode = CullMode::Back;
	friend class GraphicsEngine;
};