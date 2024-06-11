#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Vector3D.h>

class TerrainComponent : public Component
{
public:
	TerrainComponent();
	virtual ~TerrainComponent();

	void setHeightMap(const TexturePtr& heightMap);
	const TexturePtr& getHeightMap();

	void setGroundMap(const TexturePtr& groundMap);
	const TexturePtr& getGroundMap();

	void setCliffMap(const TexturePtr& cliffMap);
	const TexturePtr& getCliffMap();

	void setSize(const Vector3D& size);
	Vector3D getSize();

	f32 getHeightFromWorldPoint(const Vector3D& worldPoint);

	bool intersect(const Vector3D& pos, const Vector3D& dir, f32 distance, f32 minDistance, Vector3D& res);

private:
	void generateTerrainMesh();
	void updateData(void* data, ui32 size);
protected:
	virtual void onCreateInternal();
private:
	TexturePtr m_heightMap;
	TexturePtr m_groundMap;
	TexturePtr m_cliffMap;

	Vector3D m_size = Vector3D(512, 100, 512);

	VertexBufferPtr m_meshVb;
	IndexBufferPtr m_meshIb;
	ConstantBufferPtr m_cb;

	VertexShaderPtr m_vertexShader;
	PixelShaderPtr m_pixelShader;

	friend class GraphicsEngine;
};