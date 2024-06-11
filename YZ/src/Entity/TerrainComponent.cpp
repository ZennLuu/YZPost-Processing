#include <Entity/TerrainComponent.h>
#include <Entity/Entity.h>
#include <Entity/TransformComponent.h>

#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/ConstantBuffer.h>
#include <Graphics/Texture2D.h>
#include <Math/VertexMesh.h>
#include <Math/Math.h>

#include <Resource/Texture.h>
#include <Physics/PhysicsEngine.h>

TerrainComponent::TerrainComponent()
{
}

TerrainComponent::~TerrainComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
	m_entity->getWorld()->getGame()->getPhysicsEngine()->removeComponent(this);
}

void TerrainComponent::setHeightMap(const TexturePtr& heightMap)
{
	m_heightMap = heightMap;
}

const TexturePtr& TerrainComponent::getHeightMap()
{
	return m_heightMap;
}

void TerrainComponent::setGroundMap(const TexturePtr& groundMap)
{
	m_groundMap = groundMap;
}

const TexturePtr& TerrainComponent::getGroundMap()
{
	return m_groundMap;
}

void TerrainComponent::setCliffMap(const TexturePtr& cliffMap)
{
	m_cliffMap = cliffMap;
}

const TexturePtr& TerrainComponent::getCliffMap()
{
	return m_cliffMap;
}

void TerrainComponent::setSize(const Vector3D& size)
{
	m_size = size;
}

Vector3D TerrainComponent::getSize()
{
	return m_size;
}

f32 TerrainComponent::getHeightFromWorldPoint(const Vector3D& worldPoint)
{
	auto getPixelFromTexCoord = [&](const Vector2D& texcoord)
		{
			auto mapSize = m_heightMap->getTexture()->getSize();
			auto pixels = m_heightMap->getTexture()->getPixels();
			auto bytesPerPixel = m_heightMap->getTexture()->getBitsPerPixel() / 8;
			auto index = (ui32)(((mapSize.width * bytesPerPixel) * texcoord.y) + texcoord.x * bytesPerPixel);
			auto height = (f32)pixels[index];
			height /= 255.0f;
			return height;
		};



	auto mapSize = m_heightMap->getTexture()->getSize();
	auto scaleSize = m_size;
	auto worldPos = m_entity->getTransform()->getPosition();


	auto scale = Vector3D(scaleSize.x / mapSize.width, 0, scaleSize.z / mapSize.height);
	auto tempPoint = worldPoint;
	tempPoint = Vector3D(
		(tempPoint.x - worldPos.x) / scale.x,
		0,
		(tempPoint.z - worldPos.z) / scale.z
	);

	if (tempPoint.x < 0 || tempPoint.z < 0)
		return -50;

	auto x = (f32)(ui32)tempPoint.x;
	auto y = (f32)(ui32)tempPoint.z;

	auto deltaX = tempPoint.x - (i32)tempPoint.x;
	auto deltaY = tempPoint.z - (i32)tempPoint.z;

	auto height0 = getPixelFromTexCoord(Vector2D(x, y));
	auto height1 = getPixelFromTexCoord(Vector2D(x + 1, y));

	auto height2 = getPixelFromTexCoord(Vector2D(x, y + 1));
	auto height3 = getPixelFromTexCoord(Vector2D(x + 1, y + 1));

	auto heightX1 = Math::lerp(height0, height1, deltaX);
	auto heightX2 = Math::lerp(height2, height3, deltaX);

	auto height = Math::lerp(heightX1, heightX2, deltaY);

	return height * scaleSize.y;
}

bool TerrainComponent::intersect(const Vector3D& pos, const Vector3D& dir, f32 distance, f32 minDistance, Vector3D& res)
{
	if (distance <= 0.0f) return false;

	auto dist = 0.0f;
	auto step = distance / 10.0f;
	auto minDist = 0.1;

	while (dist <= distance)
	{
		auto f = pos + dir * dist;
		auto f2 = pos + dir * (dist + 1.0f);

		auto height0 = getHeightFromWorldPoint(f) + minDistance;
		auto height1 = getHeightFromWorldPoint(f2) + minDistance;

		auto height = Math::lerp(height0, height1, dist / distance);

		auto diff = f.y - height;

		if ((diff >= 0 && diff < minDist) || diff < 0)
		{
			res = Vector3D(f.x, height, f.z);
			return true;
		}

		dist += step;
	}

	return false;
}

void TerrainComponent::generateTerrainMesh()
{
	const ui32 w = 512;
	const ui32 h = 512;

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

	m_vertexShader = renderSystem->createVertexShader(L"Assets/Shaders/Terrain.hlsl", "vsmain");
	m_pixelShader = renderSystem->createPixelShader(L"Assets/Shaders/Terrain.hlsl", "psmain");
}

void TerrainComponent::updateData(void* data, ui32 size)
{
	auto renderSystem = m_entity->getWorld()->getGame()->getGraphicsEngine()->getRenderSystem();

	if (!m_cb)
		m_cb = renderSystem->createConstantBuffer(data, size);
	else
		m_cb->update(renderSystem->getImmediateDeviceContext(), data);
}

void TerrainComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
	m_entity->getWorld()->getGame()->getPhysicsEngine()->addComponent(this);
	generateTerrainMesh();
}