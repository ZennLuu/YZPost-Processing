#pragma once
#include <Prerequisites.h>
#include <Math/Rect.h>
#include <set>
#include <d3d11.h> 
#include <wrl.h>

#include "Graphics/ImguiManager.h"

class GraphicsEngine
{
public:
	GraphicsEngine(Game* game);
	~GraphicsEngine();

	void update();
public:
	RenderSystem* getRenderSystem();

	void addComponent(Component* component);
	void removeComponent(Component* component);

	void createPostProcessInternal();
	void resizePostProcess(const Rect& size);

private:
	std::unique_ptr<RenderSystem> m_render_system = nullptr;
	Game* m_game = nullptr;

	std::unique_ptr<ImguiManager> m_imgui;
	
	std::set<MeshComponent*> m_meshes;
	std::set<CameraComponent*> m_cameras;
	std::set<LightComponent*> m_lights;
	std::set<TerrainComponent*> m_terrains;
	std::set<WaterComponent*> m_waterComps;
	std::set<FogComponent*> m_fogs;
	std::set<TextComponent*> m_texts;
	std::set<ImageComponent*> m_images;
	std::set<PostProcessComponent*> m_postProcess;

	friend class DeviceContext;
};