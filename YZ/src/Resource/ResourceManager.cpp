#include <Resource/ResourceManager.h>
#include <Resource/Mesh.h>
#include <Resource/Texture.h>
#include <Resource/Material.h>
#include <Resource/Font.h>
#include <filesystem>

ResourceManager::ResourceManager(Game* game): m_game(game)
{
}

ResourceManager::~ResourceManager()
{
}

MeshPtr ResourceManager::createMesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size, unsigned int* index_list_data, unsigned int index_list_size, MaterialSlot* material_slot_list, unsigned int material_slot_list_size)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(
			vertex_list_data, vertex_list_size,
			index_list_data, index_list_size,
			material_slot_list, material_slot_list_size, this);
	}
	catch (...) {}

	MeshPtr mesh_ptr(mesh);
	return mesh_ptr;
}

TexturePtr ResourceManager::createTexture(const Rect& size, Texture2D::Type type)
{
	Texture* tex = nullptr;
	try
	{
		tex = new Texture(size, type, this);
	}
	catch (...) {}

	TexturePtr tex_ptr(tex);
	return tex_ptr;
}

Game* ResourceManager::getGame()
{
	return m_game;
}

ResourcePtr ResourceManager::createResourceFromFileConcrete(const wchar_t * file_path)
{
	std::filesystem::path resourcePath = file_path;

	auto ext = resourcePath.extension();

	auto it = m_map_resources.find(file_path);

	if (it != m_map_resources.end()) 
	{
		auto mat = std::dynamic_pointer_cast<Material>(it->second);
		if (mat) return std::make_shared<Material>(mat, this);
		return it->second;
	}

	if (!std::filesystem::exists(resourcePath))
		return ResourcePtr();

	ResourcePtr resPtr;

	if (!ext.compare(L".obj"))
		resPtr = std::make_shared<Mesh>(resourcePath.c_str(), this);
	else if (!ext.compare(L".jpg") || !ext.compare(L".png") || !ext.compare(L".bmp"))
		resPtr = std::make_shared<Texture>(resourcePath.c_str(), this);
	else if (!ext.compare(L".hlsl") || !ext.compare(L".fx"))
		resPtr = std::make_shared<Material>(resourcePath.c_str(), this);
	else if (!ext.compare(L".font"))
		resPtr = std::make_shared<Font>(resourcePath.c_str(), this);

	if (resPtr)
	{
		m_map_resources.emplace(file_path, resPtr);
		return resPtr;
	}

	return nullptr;
}

