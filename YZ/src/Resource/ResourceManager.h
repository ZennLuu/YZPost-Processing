#pragma once
#include <unordered_map>
#include <string>
#include <Prerequisites.h>
#include <Resource/Resource.h>
#include <Resource/Mesh.h>
#include <Math/Rect.h>
#include <Graphics/Texture2D.h>

class ResourceManager
{
public:
	ResourceManager(Game* game);
	~ResourceManager();

	template<typename T>
	std::shared_ptr<T> createResourceFromFile(const wchar_t* file_path)
	{
		return std::dynamic_pointer_cast<T>(createResourceFromFileConcrete(file_path));
	}

	MeshPtr createMesh(
		VertexMesh* vertex_list_data, unsigned int vertex_list_size,
		unsigned int* index_list_data, unsigned int index_list_size,
		MaterialSlot* material_slot_list, unsigned int material_slot_list_size
	);

	TexturePtr createTexture(const Rect& size, Texture2D::Type type);

	Game* getGame();
private:
	ResourcePtr createResourceFromFileConcrete(const wchar_t* file_path);
private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
	Game* m_game = nullptr;
};