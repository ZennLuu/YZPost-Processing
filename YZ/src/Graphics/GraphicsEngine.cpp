#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/DeviceContext.h>
#include <Graphics/SwapChain.h>
#include <Graphics/Font2D.h>

#include <Graphics/ConstantBuffer.h>
#include <Resource/ResourceManager.h>
#include <Resource/Mesh.h>
#include <Resource/Texture.h>
#include <Resource/Material.h>
#include <Resource/Font.h>

#include <Game/Game.h>
#include <Game/Display.h>

#include <Math/Matrix4x4.h>
#include <Math/Vector4D.h>

#include <Entity/TransformComponent.h>
#include <Entity/MeshComponent.h>
#include <Entity/CameraComponent.h>
#include <Entity/LightComponent.h>
#include <Entity/TerrainComponent.h>
#include <Entity/WaterComponent.h>
#include <Entity/FogComponent.h>
#include <Entity/TextComponent.h>
#include <Entity/ImageComponent.h>
#include <Entity/PostProcessComponent.h>

#include <Entity/Entity.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

__declspec(align(16))
struct CameraData
{
	Matrix4x4 view;
	Matrix4x4 proj;
	Vector4D position;
};


__declspec(align(16))
struct LightData
{
	Vector4D color;
	Vector4D direction;
	Vector4D position;
	int type;
	float intensity;
	//float radius;
};

__declspec(align(16))
struct TerrainData
{
	Vector4D size;
	f32 heightMapSize = 0.0f;
};

__declspec(align(16))
struct WaterData
{
	Vector4D size;
	f32 heightMapSize = 0.0f;
};

__declspec(align(16))
struct FogData
{
	Vector4D color;
	f32 start;
	f32 end;
	f32 enable;
};

__declspec(align(16))
struct MaterialData
{
	f32 metallic = 0.0;
	f32 roughness = 0.5;
	f32 reflectance = 0.5;
};

__declspec(align(16))
struct MulLightData 
{
	int lightnumber;
	int lightindex;
};
__declspec(align(16))
struct ConstantData
{
	Matrix4x4 world;
	float time;
	CameraData camera;
	TerrainData terrain;
	WaterData water;
	FogData fog; 
	MaterialData material;
	LightData light[16];
	MulLightData lights;
};


GraphicsEngine::GraphicsEngine(Game* game) : m_game(game)
{
	m_render_system = std::make_unique<RenderSystem>();
	m_imgui = std::make_unique<ImguiManager>();

	ImGui_ImplDX11_Init(this->m_render_system->m_d3d_device.Get(), this->m_render_system->m_imm_context.Get());
}

void GraphicsEngine::update()
{
	auto swapChain = m_game->m_display->m_swapChain;
	auto winSize = m_game->m_display->getClientSize();
	auto context = m_render_system->getImmediateDeviceContext();
	m_render_system->clearState();

	ConstantData constData = {};
	constData.time = m_game->m_totalTime;

	Vector4D fogColor = Vector4D(0, 0, 0, 0);
	
	for (auto f : m_fogs)
	{
		constData.fog.enable = true;
		fogColor = f->getColor();
		constData.fog.color = fogColor;
		constData.fog.start = f->getStart();
		constData.fog.end = f->getEnd();
	}

	if(m_postProcess.size())
	{
		for (auto p : m_postProcess)
		{
			auto renderTargetSize = p->m_render_target->getTexture()->getSize();
			//context->clearRenderTargetColor(p->m_render_target, fogColor.x, fogColor.y, fogColor.z, 1);
			context->clearRenderTargetColor(p->m_render_target, fogColor.x, fogColor.y, fogColor.z, 1);
			context->clearDepthStencil(p->m_depth_stencil);
			context->setRenderTarget(p->m_render_target, p->m_depth_stencil);
			context->setViewportSize(renderTargetSize.width, renderTargetSize.height);
		}
	}
	else 
	{
		context->clearRenderTargetColor(swapChain, fogColor.x, fogColor.y, fogColor.z, 1);
		context->setViewportSize(winSize.width, winSize.height);
	}

	for (auto c : m_cameras)
	{
		auto t = c->getEntity()->getTransform();
		constData.camera.position = t->getPosition();
		c->setScreenArea(winSize);
		c->getViewMatrix(constData.camera.view);
		c->getProjectionMatrix(constData.camera.proj);
	}

	int lightnumber = 0;
	for (auto l : m_lights)
	{
		auto t = l->getEntity()->getTransform();
		Matrix4x4 w;
		t->getWorldMatrix(w);
		constData.light[lightnumber].type = (int)l->getType();
		constData.light[lightnumber].direction = w.getZDirection();
		constData.light[lightnumber].position = t->getPosition();
		constData.light[lightnumber].intensity = l->getIntensity();
		//constData.light.radius = l->getRadius();
		constData.light[lightnumber].color = l->getColor();

		auto m = l->getEntity()->getComponent<MeshComponent>();
		
		if (m != nullptr) 
		{
			auto transform = m->getEntity()->getTransform();

			transform->getWorldMatrix(constData.world);

			const auto materials = m->getMaterials();

			auto mesh = m->getMesh().get();

			context->setVertexBuffer(mesh->m_vertex_buffer);
			context->setIndexBuffer(mesh->m_index_buffer);

			constData.lights.lightindex = lightnumber;

			for (auto i = 0; i < mesh->getNumMaterialSlots(); i++)
			{
				if (i >= materials.size()) break;
				auto mat = materials[i].get();

				m_render_system->setCullMode(mat->getCullMode());

				constData.material.metallic = mat->m_metallic;
				constData.material.roughness = mat->m_roughness;
				constData.material.reflectance = mat->m_reflectance;

				mat->setData(&constData, sizeof(ConstantData));
				context->setConstantBuffer(mat->m_constant_buffer);

				context->setVertexShader(mat->m_vertex_shader);
				context->setPixelShader(mat->m_pixel_shader);

				context->setTexture(&mat->m_vec_textures[0], (unsigned int)mat->m_vec_textures.size());

				auto slot = mesh->getMaterialSlot(i);
				context->drawIndexedTriangleList((unsigned int)slot.num_indices, (unsigned int)slot.start_index, 0);
			}
		}


		lightnumber++;
	}
	constData.lights.lightnumber = lightnumber;

	for (auto m : m_meshes)
	{
		auto isLight = m->getEntity()->getComponent<LightComponent>();

		if (isLight != nullptr)
			continue;

		auto transform = m->getEntity()->getTransform();

		transform->getWorldMatrix(constData.world);

		auto pos = transform->getPosition();
		auto camPos = constData.camera.position;

		auto distance = Vector3D::distance(pos, Vector3D(camPos.x, camPos.y, camPos.z));
		
		//auto camDir = Vector3D::normalize(constData.camera.view.getTranslation() + constData.camera.view.getYDirection() + constData.camera.view.getZDirection());
		//auto dir = Vector3D(pos.x - camPos.x, pos.y - camPos.y, pos.z - camPos.z);

		//auto angle = Vector3D::angle(dir, camDir);

		if (distance > 200) continue;
		//if (angle > 45) continue;
		
		auto mesh = m->getMesh().get();
		const auto materials = m->getMaterials();


		context->setVertexBuffer(mesh->m_vertex_buffer);
		context->setIndexBuffer(mesh->m_index_buffer);

		constData.lights.lightindex = m->getLightParent();

		for (auto i = 0; i < mesh->getNumMaterialSlots(); i++)
		{
			if (i >= materials.size()) break;
			auto mat = materials[i].get();
			
			m_render_system->setCullMode(mat->getCullMode());

			constData.material.metallic = mat->m_metallic;
			constData.material.roughness = mat->m_roughness;
			constData.material.reflectance = mat->m_reflectance;

			mat->setData(&constData, sizeof(ConstantData));
			context->setConstantBuffer(mat->m_constant_buffer);

			context->setVertexShader(mat->m_vertex_shader);
			context->setPixelShader(mat->m_pixel_shader);

			context->setTexture(&mat->m_vec_textures[0], (unsigned int)mat->m_vec_textures.size());

			auto slot = mesh->getMaterialSlot(i);
			context->drawIndexedTriangleList((unsigned int)slot.num_indices, (unsigned int)slot.start_index, 0);
		}
	}

	for (auto t : m_terrains)
	{
		auto transform = t->getEntity()->getTransform();
		transform->getWorldMatrix(constData.world);
		constData.terrain.size = t->getSize();
		constData.terrain.heightMapSize = (f32)t->getHeightMap()->getTexture()->getSize().width;

		context->setVertexBuffer(t->m_meshVb);
		context->setIndexBuffer(t->m_meshIb);

		m_render_system->setCullMode(CullMode::Back);
		t->updateData(&constData, sizeof(constData));
		context->setConstantBuffer(t->m_cb);

		context->setVertexShader(t->m_vertexShader);
		context->setPixelShader(t->m_pixelShader);

		Texture2DPtr terrainTextures[3];
		terrainTextures[0] = t->getHeightMap()->getTexture();
		terrainTextures[1] = t->getGroundMap()->getTexture();
		terrainTextures[2] = t->getCliffMap()->getTexture();

		context->setTexture(terrainTextures, 3);

		context->drawIndexedTriangleList((ui32)t->m_meshIb->getSizeIndexList(), 0, 0);
	}


	for (auto w : m_waterComps)
	{
		auto transform = w->getEntity()->getTransform();
		transform->getWorldMatrix(constData.world);
		constData.water.size = w->getSize();
		constData.water.heightMapSize = (f32)w->getWaveHeightMap()->getTexture()->getSize().width;

		context->setVertexBuffer(w->m_meshVb);
		context->setIndexBuffer(w->m_meshIb);

		m_render_system->setCullMode(CullMode::Back);
		w->updateData(&constData, sizeof(constData));
		context->setConstantBuffer(w->m_cb);

		context->setVertexShader(w->m_vertexShader);
		context->setPixelShader(w->m_pixelShader);

		Texture2DPtr waterTextures[1];
		waterTextures[0] = w->getWaveHeightMap()->getTexture();

		context->setTexture(waterTextures, 1);

		context->drawIndexedTriangleList((ui32)w->m_meshIb->getSizeIndexList(), 0, 0);
	}

	//Post Process
	if (m_postProcess.size())
	{
		for (auto p : m_postProcess)
		{
			context->clearRenderTargetColor(swapChain, 0, 0, 0, 1);
			context->setViewportSize(winSize.width, winSize.height);

			context->setVertexBuffer(p->m_meshVb);
			context->setIndexBuffer(p->m_meshIb);

			p->setData(&constData, sizeof(constData));
			context->setConstantBuffer(p->m_cb);

			context->setVertexShader(p->m_vertexShader);
			context->setPixelShader(p->m_pixelShader);

			m_render_system->setCullMode(CullMode::Front);

			Texture2DPtr textures[2];

			textures[0] = p->m_render_target->getTexture();
			textures[1] = p->m_depth_stencil->getTexture();

			context->setTexture(textures, 2);

			context->drawIndexedTriangleList((ui32)p->m_meshIb->getSizeIndexList(), 0, 0);
		}
	}

	// RENDERING UI COMPONENTS //
	// ----------------------- //
	for (auto i : m_images)
	{
		auto transform = i->getEntity()->getTransform();
		auto pos = transform->getPosition();

		auto texture = i->getImage()->getTexture();
		auto size = i->getSize();
		
		m_render_system->drawImage(texture, { (i32)pos.x,(i32)pos.y,size.width,size.height });
	}

	for (auto t : m_texts)
	{
		auto transform = t->getEntity()->getTransform();
		auto pos = transform->getPosition();

		auto font = t->getFont()->getFont();

		font->m_batch->Begin();
		font->m_font->DrawString(font->m_batch.get(), t->getText(), DirectX::XMFLOAT2(pos.x, pos.y));
		font->m_batch->End();
	}

	m_game->dramImgui();

	swapChain->present(true);
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system.get();
}

void GraphicsEngine::addComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.emplace(c);
	else if (auto c = dynamic_cast<CameraComponent*>(component))
	{
		if (!m_cameras.size()) m_cameras.emplace(c);
	}
	else if (auto c = dynamic_cast<LightComponent*>(component))
	{
		/*if (!m_lights.size())*/ m_lights.emplace(c);
	}
	else if (auto c = dynamic_cast<TerrainComponent*>(component))
	{
		if (!m_terrains.size()) m_terrains.emplace(c);
	}
	else if (auto c = dynamic_cast<WaterComponent*>(component))
	{
		if (!m_waterComps.size()) m_waterComps.emplace(c);
	}
	else if (auto c = dynamic_cast<FogComponent*>(component))
	{
		if (!m_fogs.size()) m_fogs.emplace(c);
	}
	else if (auto c = dynamic_cast<TextComponent*>(component))
	{
		m_texts.emplace(c);
	}
	else if (auto c = dynamic_cast<ImageComponent*>(component))
	{
		m_images.emplace(c);
	}
	else if (auto c = dynamic_cast<PostProcessComponent*>(component))
	{
		if (!m_postProcess.size()) m_postProcess.emplace(c);
	}
}

void GraphicsEngine::removeComponent(Component* component)
{
	if (auto c = dynamic_cast<MeshComponent*>(component))
		m_meshes.erase(c);
	else if (auto c = dynamic_cast<CameraComponent*>(component))
		m_cameras.erase(c);
	else if (auto c = dynamic_cast<LightComponent*>(component))
		m_lights.erase(c);
	else if (auto c = dynamic_cast<TerrainComponent*>(component))
		m_terrains.erase(c);
	else if (auto c = dynamic_cast<WaterComponent*>(component))
		m_waterComps.erase(c);
	else if (auto c = dynamic_cast<FogComponent*>(component))
		m_fogs.erase(c);
	else if (auto c = dynamic_cast<TextComponent*>(component))
		m_texts.erase(c);
	else if (auto c = dynamic_cast<ImageComponent*>(component))
		m_images.erase(c);
	else if (auto c = dynamic_cast<PostProcessComponent*>(component))
		m_postProcess.erase(c);
}

void GraphicsEngine::createPostProcessInternal()
{
}

void GraphicsEngine::resizePostProcess(const Rect& size)
{
	for (auto p : m_postProcess) 
	{
		p->resize(size);
	}
}

GraphicsEngine::~GraphicsEngine()
{
	ImGui_ImplDX11_Shutdown();
}