#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Vector3D.h>

class WaterComponent : public Component
{
public:
	WaterComponent();
	virtual ~WaterComponent();

	void setWaveHeightMap(const TexturePtr& heightMap);
	const TexturePtr& getWaveHeightMap();

	void setSize(const Vector3D& size);
	const Vector3D& getSize();
private:
	void generateMesh();
	void updateData(void* data, ui32 size);

protected:
	virtual void onCreateInternal();
private:
	TexturePtr m_waveHeightMap;

	Vector3D m_size = Vector3D(2048, 8, 2048);

	VertexBufferPtr m_meshVb;
	IndexBufferPtr m_meshIb;
	ConstantBufferPtr m_cb;

	VertexShaderPtr m_vertexShader;
	PixelShaderPtr m_pixelShader;

	friend class GraphicsEngine;
};

