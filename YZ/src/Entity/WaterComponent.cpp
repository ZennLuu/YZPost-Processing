#include <Entity/WaterComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/ConstantBuffer.h>
#include <Math/VertexMesh.h>

WaterComponent::WaterComponent()
{
}

WaterComponent::~WaterComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void WaterComponent::setWaveHeightMap(const TexturePtr& heightMap)
{
	m_waveHeightMap = heightMap;
}

const TexturePtr& WaterComponent::getWaveHeightMap()
{
	return m_waveHeightMap;
}

void WaterComponent::setSize(const Vector3D& size)
{
	m_size = size;
}

const Vector3D& WaterComponent::getSize()
{
	return m_size;
}

void WaterComponent::generateMesh()
{
	const ui32 w = m_size.x / 2;
	const ui32 h = m_size.z / 2;

	const ui32 ww = w - 1;
	const ui32 hh = h - 1;


	VertexMesh* terrainMeshVertices = new VertexMesh[w * h];
	ui32* terrainMeshIndices = new ui32[ww * hh * 6];

	auto i = 0;
	for (ui32 x = 0; x < w; x++)
	{
		for (ui32 y = 0; y < h; y++)
		{
			terrainMeshVertices[y * w + x] = {
				Vector3D((f32)x / (f32)ww,0,(f32)y / (f32)hh),
				Vector2D((f32)x / (f32)ww,(f32)y / (f32)hh),
				Vector3D(),
				Vector3D(),
				Vector3D()
			};

			if (x < ww && y < hh)
			{
				terrainMeshIndices[i] = (y + 1) * w + (x);
				terrainMeshIndices[i + 1] = (y)*w + (x);
				terrainMeshIndices[i + 2] = (y)*w + (x + 1);

				terrainMeshIndices[i + 3] = (y)*w + (x + 1);
				terrainMeshIndices[i + 4] = (y + 1) * w + (x + 1);
				terrainMeshIndices[i + 5] = (y + 1) * w + (x);
				i += 6;
			}
		}
	}

	auto renderSystem = m_entity->getWorld()->getGame()->getGraphicsEngine()->getRenderSystem();
	m_meshVb = renderSystem->createVertexBuffer(terrainMeshVertices, sizeof(VertexMesh), w * h);
	m_meshIb = renderSystem->createIndexBuffer(terrainMeshIndices, ww * hh * 6);

	m_vertexShader = renderSystem->createVertexShader(L"Assets/Shaders/Water.hlsl", "vsmain");
	m_pixelShader = renderSystem->createPixelShader(L"Assets/Shaders/Water.hlsl", "psmain");
}

void WaterComponent::updateData(void* data, ui32 size)
{
	auto renderSystem = m_entity->getWorld()->getGame()->getGraphicsEngine()->getRenderSystem();

	if (!m_cb)
		m_cb = renderSystem->createConstantBuffer(data, size);
	else
		m_cb->update(renderSystem->getImmediateDeviceContext(), data);
}

void WaterComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
	generateMesh();
}
