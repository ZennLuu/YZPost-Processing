#pragma once
#include <Resource/Resource.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/IndexBuffer.h>
#include <Math/Vector3D.h>
#include <Math/Vector2D.h>
#include <Math/VertexMesh.h>
#include <vector>

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path, ResourceManager* manager);
	Mesh(VertexMesh* vertex_list_data, ui32 vertex_list_size,
		unsigned int* index_list_data, ui32 index_list_size,
		MaterialSlot* material_slot_list, ui32 material_slot_list_size, ResourceManager* manager);
	~Mesh();

	MaterialSlot getMaterialSlot(ui32 slot);
	size_t getNumMaterialSlots();
private:
	void computeTangents(
		const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,
		const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
		Vector3D& tangent, Vector3D& binormal);
private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_mat_slots;
private:
	friend class DeviceContext;
	friend class GraphicsEngine;
};