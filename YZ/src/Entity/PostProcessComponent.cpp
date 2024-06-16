#include <Entity/PostProcessComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Game/Display.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/Texture2D.h>
#include <Graphics/ConstantBuffer.h>

#include <Resource/ResourceManager.h>

#include <Math/VertexMesh.h>


PostProcessComponent::PostProcessComponent()
{
}

PostProcessComponent::PostProcessComponent(Type type)
{
	m_type = type;
}

PostProcessComponent::~PostProcessComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void PostProcessComponent::setShader(const wchar_t* file_path)
{
	auto renderSystem = m_entity->getWorld()->getGame()->getGraphicsEngine()->getRenderSystem();
	m_vertexShader = renderSystem->createVertexShader(file_path, "vsmain");
	m_pixelShader = renderSystem->createPixelShader(file_path, "psmain");
}

void PostProcessComponent::setData(void* data, ui32 size)
{
	auto renderSystem = m_entity->getWorld()->getGame()->getGraphicsEngine()->getRenderSystem();

	if (!m_cb)
		m_cb = renderSystem->createConstantBuffer(data, size);
	else
		m_cb->update(renderSystem->getImmediateDeviceContext(), data);
}

void PostProcessComponent::resize(const Rect& window)
{
	auto resManager = m_entity->getWorld()->getGame()->getResourceManager();
	m_render_target = resManager->createTexture(window, Texture2D::Type::HDR);
	m_depth_stencil = resManager->createTexture(window, Texture2D::Type::DepthStencil);
}

void PostProcessComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);


	VertexMesh* quad_vertex_list = new VertexMesh[4];
	quad_vertex_list[0] = VertexMesh(Vector3D(-1, -1, 0), Vector2D(0, 1), Vector3D(), Vector3D(), Vector3D());
	quad_vertex_list[1] = VertexMesh(Vector3D(-1, 1, 0), Vector2D(0, 0), Vector3D(), Vector3D(), Vector3D());
	quad_vertex_list[2] = VertexMesh(Vector3D(1, 1, 0), Vector2D(1, 0), Vector3D(), Vector3D(), Vector3D());
	quad_vertex_list[3] = VertexMesh(Vector3D(1, -1, 0), Vector2D(1, 1), Vector3D(), Vector3D(), Vector3D());

	ui32* quad_index_list = new ui32[6];
	quad_index_list[0] = 0;
	quad_index_list[1] = 1;
	quad_index_list[2] = 2;
	quad_index_list[3] = 2;
	quad_index_list[4] = 3;
	quad_index_list[5] = 0;

	auto resManager = m_entity->getWorld()->getGame()->getResourceManager();

	auto winSize = m_entity->getWorld()->getGame()->getDisplay()->getClientSize();

	//winSize.width = winSize.width / 8;
	//winSize.height = winSize.height / 8;

	m_render_target = resManager->createTexture(winSize, Texture2D::Type::HDR);
	m_depth_stencil = resManager->createTexture(winSize, Texture2D::Type::DepthStencil);

	auto renderSystem = m_entity->getWorld()->getGame()->getGraphicsEngine()->getRenderSystem();

	m_meshVb = renderSystem->createVertexBuffer(quad_vertex_list, sizeof(VertexMesh), 4);
	m_meshIb = renderSystem->createIndexBuffer(quad_index_list, 6);

	m_vertexShader = renderSystem->createVertexShader(L"Assets/Shaders/DefaultPostProcess.hlsl", "vsmain");
	m_pixelShader = renderSystem->createPixelShader(L"Assets/Shaders/DefaultPostProcess.hlsl", "psmain");
}
