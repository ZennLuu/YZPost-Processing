#include <Resource/Mesh.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <Math/VertexMesh.h>
#include <Resource/ResourceManager.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <filesystem>

Mesh::Mesh(const wchar_t* full_path, ResourceManager* manager): Resource(full_path, manager)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	auto inputfile = std::filesystem::path(full_path).string();

	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));


	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());

	if (!err.empty()) throw std::exception("Mesh not created successfully");

	if (!res) throw std::exception("Mesh not created successfully");


	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;
	
	size_t vertex_buffer_size = 0;


	for (size_t s = 0; s < shapes.size(); s++)
	{
		vertex_buffer_size += shapes[s].mesh.indices.size();
	}
	
	
	list_vertices.reserve(vertex_buffer_size);
	list_indices.reserve(vertex_buffer_size);


	m_mat_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_mat_slots[m].start_index = index_global_offset;
		m_mat_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];
				
				if (shapes[s].mesh.material_ids[f] != m)
				{
					index_offset += num_face_verts;
					continue;
				}

				Vector3D vertices_face[3];
				Vector2D texcoord_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size()) {
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					vertices_face[v] = Vector3D(vx, vy, vz);
					texcoord_face[v] = Vector2D(tx, ty);
				}

				Vector3D tangent, binormal;

				computeTangents(
					vertices_face[0], vertices_face[1], vertices_face[2], 
					texcoord_face[0], texcoord_face[1], texcoord_face[2], 
					tangent, binormal);

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size()) {
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = 0;
					tinyobj::real_t ny = 0;
					tinyobj::real_t nz = 0;
					if(attribs.normals.size())
					{
						nx = attribs.normals[index.normal_index * 3 + 0];
						ny = attribs.normals[index.normal_index * 3 + 1];
						nz = -attribs.normals[index.normal_index * 3 + 2];
					}

					Vector3D v_tangent, v_binormal;
					v_binormal = Vector3D::cross(Vector3D(nx, ny, nz), tangent);
					v_tangent = Vector3D::cross(v_binormal, Vector3D(nx, ny, nz));



					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);

					list_indices.push_back((unsigned int)index_global_offset + v);
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		m_mat_slots[m].num_indices = index_global_offset - m_mat_slots[m].start_index;
	}

	auto rsys = m_manager->getGame()->getGraphicsEngine()->getRenderSystem();

	m_vertex_buffer = rsys->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
		(UINT)list_vertices.size());
	m_index_buffer = rsys->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::Mesh(VertexMesh* vertex_list_data, ui32 vertex_list_size, unsigned int* index_list_data, ui32 index_list_size, MaterialSlot* material_slot_list, ui32 material_slot_list_size, ResourceManager* manager): Resource(L"", manager)
{
	auto rsys = m_manager->getGame()->getGraphicsEngine()->getRenderSystem();

	m_vertex_buffer = rsys->createVertexBuffer(vertex_list_data, sizeof(VertexMesh),
		(UINT)vertex_list_size);
	m_index_buffer = rsys->createIndexBuffer(index_list_data, (UINT)index_list_size);

	m_mat_slots.resize(material_slot_list_size);

	for (ui32 i = 0; i < material_slot_list_size; i++) {
		m_mat_slots[i] = material_slot_list[i];
	}
}


Mesh::~Mesh()
{
}

MaterialSlot Mesh::getMaterialSlot(ui32 slot)
{
	return m_mat_slots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return m_mat_slots.size();
}

void Mesh::computeTangents(const Vector3D& v0, 
	const Vector3D& v1, 
	const Vector3D& v2, 
	const Vector2D& t0, 
	const Vector2D& t1, 
	const Vector2D& t2, 
	Vector3D& tangent, Vector3D& binormal)
{
	Vector3D deltaPos1 = v1 - v0;
	Vector3D deltaPos2 = v2 - v0;

	Vector2D deltaUV1 = t1 - t0;
	Vector2D deltaUV2 = t2 - t0;


	f32 r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
	tangent = Vector3D::normalize(tangent);
	binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);
	binormal = Vector3D::normalize(binormal);
}
