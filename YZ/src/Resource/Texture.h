#pragma once
#include <Resource/Resource.h>
#include <Math/Rect.h>
#include <Graphics/Texture2D.h>

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path, ResourceManager* manager);
	Texture(const Rect& size, Texture2D::Type type, ResourceManager* manager);
	const Texture2DPtr& getTexture();
private:
	Texture2DPtr m_texture;

	friend class Material;
};