#include "MyGame.h"
#include "Player.h"
#include <time.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

MyGame::MyGame()
{
}

MyGame::~MyGame()
{
}

void MyGame::onCreate()
{
	Game::onCreate();

	srand((ui32)time(nullptr));

	// Loading font
	FontUtility fontUtility;
	fontUtility.createFont(L"Bahnschrift", 16, L"Assets\\Fonts\\");

	auto resourceManager = getResourceManager();
	
	// Adding resources

	// Models
	auto ball = resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/sphere_hq.obj");
	auto cube = resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/cume.obj");
	auto plane = resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/plane.obj");
	auto suzanna = resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/suzanna.obj");

	// Textures
	auto skyTex = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/sky.jpg");

	auto tex1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/brick_d.jpg");
	auto tex2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/brick_n.jpg");

	auto mtex1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M3/color.jpg");
	auto mtex2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M3/normal.jpg");
	auto mtex3 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M3/metallnes.jpg");
	auto mtex4 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M3/roughness.jpg");
	auto mtex5 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M3/displacement.jpg");

	auto mtex1_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/color.jpg");
	auto mtex2_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/normal.jpg");
	auto mtex3_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/metallnes.jpg");
	auto mtex4_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/roughness.jpg");
	auto mtex5_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/displacement.jpg");

	auto mtex1_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/color.jpg");
	auto mtex2_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/normal.jpg");
	auto mtex3_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/metallnes.jpg");
	auto mtex4_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/roughness.jpg");
	auto mtex5_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/displacement.jpg");

	auto cliff = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/ground.jpg");
	auto ground = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/sand.jpg");

	auto waveHeightMap = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/waveHeightMap.png");
	auto heightMap = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/height_map.png");

	auto font = resourceManager->createResourceFromFile<Font>(L"Assets/Fonts/Bahnschrift.font");

	auto cross = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/UI/cross.png");

	// Creating materials
	auto skyMat = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/SkyBox.hlsl");
	skyMat->addTexture(skyTex);
	skyMat->setCullMode(CullMode::Front);

	auto lMat = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/LightShader.hlsl");
	lMat->addTexture(skyTex);
	lMat->setCullMode(CullMode::Back);

	mat1 = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/Base2.hlsl");
	mat1->addTexture(mtex1_2);
	mat1->addTexture(mtex2_2);
	mat1->addTexture(mtex3_2);
	mat1->addTexture(mtex4_2);
	mat1->addTexture(mtex5_2);
	mat1->setCullMode(CullMode::Back);

	mat2 = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/Base2.hlsl");
	mat2->addTexture(mtex1);
	mat2->addTexture(mtex2);
	mat2->addTexture(mtex3);
	mat2->addTexture(mtex4);
	mat2->addTexture(mtex5);
	mat2->setCullMode(CullMode::Back);

	mat3 = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/Base2.hlsl");
	mat3->addTexture(mtex1_1);
	mat3->addTexture(mtex2_1);
	mat3->addTexture(mtex3_1);
	mat3->addTexture(mtex4_1);
	mat3->addTexture(mtex5_1);
	mat3->setCullMode(CullMode::Back);

	// Creating entities with specific components
	//post process
	{
		// creating entity
		auto entity = getWorld()->createEntity<Entity>();
		
		// creating post-process component
		m_postProcess = entity->createComponent<PostProcessComponent>();
		// setting shader for post process component(will compile this shader at function call);
		m_postProcess->setShader(L"Assets/Shaders/DefaultPostProcess.hlsl");
	}

	//skybox
	{
		m_skybox = getWorld()->createEntity<Entity>();
		auto meshComp = m_skybox->createComponent<MeshComponent>();
		// setting mesh to mesh component
		meshComp->setMesh(ball);
		// setting material
		meshComp->addMaterial(skyMat);
		
		// transforming
		auto transform = m_skybox->getTransform();
		transform->setScale(Vector3D(1000, 1000, 1000));
	}	
	
	//ball
	{

		m_s1 = getWorld()->createEntity<Entity>();
		auto meshComp = m_s1->createComponent<MeshComponent>();
		meshComp->setMesh(plane);
		meshComp->addMaterial(mat3);
		auto transform = m_s1->getTransform();
		transform->setPosition(Vector3D(0, 0, 1));
		transform->setScale(Vector3D(10, 10, 10));
	}
	
	//ball
	{

		m_s2 = getWorld()->createEntity<Entity>();
		auto meshComp = m_s2->createComponent<MeshComponent>();
		meshComp->setMesh(cube);
		meshComp->addMaterial(mat2);
		auto transform = m_s2->getTransform();
		transform->setPosition(Vector3D(3, 3, 1));
	}	
	
	//ball
	{

		m_s3 = getWorld()->createEntity<Entity>();
		auto meshComp = m_s3->createComponent<MeshComponent>();
		meshComp->setMesh(ball);
		meshComp->addMaterial(mat1);
		auto transform = m_s3->getTransform();
		transform->setPosition(Vector3D(-3, 3, 1));
	}

	//terrain
	{
		auto entity = getWorld()->createEntity<Entity>();
		auto terrainComp = entity->createComponent<TerrainComponent>();
		
		// setting height map for terrain
		terrainComp->setHeightMap(heightMap);
		
		// setting textures to use on terrain
		terrainComp->setCliffMap(cliff);
		terrainComp->setGroundMap(ground);

		auto transform = entity->getTransform();
		transform->setScale(Vector3D(1, 1, 1));
		transform->setPosition(Vector3D(50, -10, 50));
	}

	//sea
	{
		auto entity = getWorld()->createEntity<Entity>();
		auto waterComponent = entity->createComponent<WaterComponent>();
		
		// setting height map for waves
		waterComponent->setWaveHeightMap(waveHeightMap);

		auto transform = entity->getTransform();
		transform->setPosition(Vector3D(-768, -10, -768));
	}

	//fog
	//{
	//	auto entity = getWorld()->createEntity<Entity>();
	//	auto fogComponent = entity->createComponent<FogComponent>();
	//	fogComponent->setData(Vector4D(0.8f, 0.9f, 1.0f, 1.0f), 1000, 10000);
	//}

	//light 1 (combining light and mesh component to see position of light)
	{
		m_light = getWorld()->createEntity<Entity>();
		auto lightComponent = m_light->createComponent<LightComponent>();
		auto meshComponent = m_light->createComponent<MeshComponent>();
		meshComponent->setMesh(ball);
		meshComponent->addMaterial(lMat);
		lightComponent->setColor(Vector4D(1.0f, 1.0f, 1.0f, 1));
		lightComponent->setType(LightComponent::LightType::Point);
		m_light->getTransform()->setRotation(Vector3D(-0.707f, 0, 0));
		//m_light->getTransform()->setPosition(Vector3D(-4, 5, 0));
		m_light->getTransform()->setScale(Vector3D(0.05, 0.05, 0.05));
	}	

	//light 2
	{
		m_light2 = getWorld()->createEntity<Entity>();
		auto lightComponent = m_light2->createComponent<LightComponent>();
		auto meshComponent = m_light2->createComponent<MeshComponent>();
		meshComponent->setMesh(ball);
		meshComponent->addMaterial(lMat);
		lightComponent->setColor(Vector4D(1.0f, 0.0f, 1.0f, 1));
		lightComponent->setType(LightComponent::LightType::Directional);
		m_light2->getTransform()->setRotation(Vector3D(-0.707f, 0, 0));
		m_light2->getTransform()->setPosition(Vector3D(0, 4, 0));
		m_light2->getTransform()->setScale(Vector3D(0.05, 0.05, 0.05));
	}

	//light 3
	{
		m_light3 = getWorld()->createEntity<Entity>();
		auto lightComponent = m_light3->createComponent<LightComponent>();
		auto meshComponent = m_light3->createComponent<MeshComponent>();
		meshComponent->setMesh(ball);
		meshComponent->addMaterial(lMat);
		lightComponent->setColor(Vector4D(0.0f, 1.0f, 0.0f, 1));
		lightComponent->setType(LightComponent::LightType::Spot);
		m_light3->getTransform()->setRotation(Vector3D(1.650f, 0, 0));
		m_light3->getTransform()->setPosition(Vector3D(0, 6, 0));
		m_light3->getTransform()->setScale(Vector3D(0.05, 0.05, 0.05));
	}

	//text
	{
		m_text = getWorld()->createEntity<Entity>();
		auto text = m_text->createComponent<TextComponent>();
		
		// setting font for text
		text->setFont(font);
		text->setText(L"");
		m_text->getTransform()->setPosition(Vector3D(10, 10, 0));

	}

	//text2
	{
		m_guide = getWorld()->createEntity<Entity>();
		auto text = m_guide->createComponent<TextComponent>();
		text->setFont(font);
		text->setText(L"");
		m_guide->getTransform()->setPosition(Vector3D(10, 10, 0));

	}

	//cross
	{
		m_cross = getWorld()->createEntity<Entity>();
		auto image = m_cross->createComponent<ImageComponent>();
		image->setImage(cross);
		image->setSize({ 0,0,50,50 });
		auto clientSize = m_cross->getWorld()->getGame()->getDisplay()->getClientSize();
		m_cross->getTransform()->setPosition(Vector3D((f32)(clientSize.width / 2.0f) - 25.0f, (f32)(clientSize.height / 2.0f) - 25.0f, 0));
	}

	m_player = getWorld()->createEntity<Player>();
}

// update data each frame
void MyGame::onUpdate(f32 deltaTime)
{
	deltaTime = deltaTime * m_speed;
	Game::onUpdate(deltaTime);
	m_rotation += 1.57f * deltaTime/ 2;

	//m_s1->getTransform()->setRotation(Vector3D(m_rotation / 4, m_rotation, 0.0));
	m_s2->getTransform()->setRotation(Vector3D(0.0, m_rotation, 0.0));
	m_s3->getTransform()->setRotation(Vector3D(0.0, m_rotation, 0.0));

	//m_terrain->getTransform()->setRotation(Vector3D(0.0, m_rotation, 0));
	//m_entity->getTransform()->setRotation(Vector3D(0.707f, m_rotation, 0));

	m_skybox->getTransform()->setPosition(m_player->getTransform()->getPosition());

	auto clientSize = getDisplay()->getClientSize();
	auto textComp1 = m_text->getComponent<TextComponent>();
	auto textBounds1 = textComp1->getBounds();
	auto textComp2 = m_guide->getComponent<TextComponent>();
	auto textBounds2 = textComp2->getBounds();

	auto playerPos = m_player->getTransform()->getPosition();

	auto infoText = std::wstringstream();
	auto infoText2 = std::wstringstream();

	infoText << L"Screen Size W: " << (int)clientSize.width << L" H: " << (int)clientSize.height << L"\n";
	infoText << L"Player coord X: " << (int)playerPos.x << L" Y: " << (int)playerPos.y << L" Z: " << (int)playerPos.z << L"\n";

	textComp1->setText(infoText.str().c_str());

	infoText2 << L"Lock cursor: Escape\n";
	infoText2 << L"Jump: Space\n";
	infoText2 << L"Move: WASD Shift: Accelerate\n";
	infoText2 << L"1 - Default PostProcess, 2 - Chromatic Abberation, 3 - Vignette, 4 - PS1\n";
	infoText2 << L"5 - Grayscale, 6 - Black and White, 7 - HDR, 8 - Edge detection\n";

	textComp2->setText(infoText2.str().c_str());
	m_guide->getTransform()->setPosition(Vector3D(
		(f32)(10),
		(f32)(clientSize.height - textBounds2.height - 10),
		0
	));

	// listen for key events
	if (getInputSystem()->isKeyUp(Key::_1))
	{
		m_postProcess->setShader(L"Assets/Shaders/DefaultPostProcess.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::_2))
	{
		m_postProcess->setShader(L"Assets/Shaders/ChromaticAbberation.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::_3))
	{
		m_postProcess->setShader(L"Assets/Shaders/Vignette.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::_4))
	{
		m_postProcess->setShader(L"Assets/Shaders/PS1.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::_5))
	{
		m_postProcess->setShader(L"Assets/Shaders/Grayscale.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::_6))
	{
		m_postProcess->setShader(L"Assets/Shaders/BlackWhite.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::_7))
	{
		m_postProcess->setShader(L"Assets/Shaders/HDR.hlsl");
	}    
	if (getInputSystem()->isKeyUp(Key::_8))
	{
		m_postProcess->setShader(L"Assets/Shaders/EdgeDetection.hlsl");
	}
	if (getInputSystem()->isKeyUp(Key::M))
	{
		m_toggleimgui = !m_toggleimgui;
	}

}

// ImGui menus setup
void MyGame::dramImgui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static char buffer[1024];

	const char* type[3] =
	{
		"Directional",
		"Point",
		"Spot"
	};

	const char* lights[3] =
	{
		"Light 1",
		"Light 2",
		"Light 3"
	};

	Entity* l_array[3] =
	{
		m_light,
		m_light2,
		m_light3
	};

	static int light_id = 0;
	static int type_id = 1;

	if (m_toggleimgui)
	{
		if (ImGui::Begin("Speed"))
		{
			ImGui::Text("App avarage: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::SliderFloat("Speed factor", &m_speed, 0.0f, 10.0f);
			if (ImGui::Combo("Light", &light_id, lights, 3))
			{
				auto t = l_array[light_id]->getTransform();
				auto l = l_array[light_id]->getComponent<LightComponent>();
				type_id = (int)l->getType();
				m_x = t->getPosition().x;
				m_y = t->getPosition().y;
				m_z = t->getPosition().z;
				m_intensity = l->getIntensity();
				m_color[0] = l->getColor().x;
				m_color[1] = l->getColor().y;
				m_color[2] = l->getColor().z;
				m_rotationL[0] = t->getRotation().x;
				m_rotationL[1] = t->getRotation().y;
				m_rotationL[2] = t->getRotation().z;

			}
			ImGui::Combo("Light type", &type_id, type, 3);
			ImGui::ColorEdit3("Color", m_color);
			ImGui::SliderFloat3("Direction", m_rotationL, -3.1415, 3.1415);
			ImGui::SliderFloat("Light X", &m_x, -10.0f, 10.0f);
			ImGui::SliderFloat("Light Y", &m_y, -10.0f, 10.0f);
			ImGui::SliderFloat("Light Z", &m_z, -10.0f, 10.0f);
			ImGui::SliderFloat("Light Intensity", &m_intensity, 0.0f, 10.0f);
			if (ImGui::Button("Reset"))
			{
				m_speed = 1.0;
				m_x = 0.0;
				m_y = 5.0;
				m_z = 0.0;
				m_intensity = 1.0;
				m_color[0] = 1.0;
				m_color[1] = 1.0;
				m_color[2] = 1.0;
				m_rotationL[0] = 0.0;
				m_rotationL[1] = 0.0;
				m_rotationL[2] = 0.0;
			}
		}
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	auto light = l_array[light_id];

	switch (type_id)
	{
	case 0:
		light->getComponent<LightComponent>()->setType(LightComponent::LightType::Directional);
		break;
	case 1:
		light->getComponent<LightComponent>()->setType(LightComponent::LightType::Point);
		break;
	case 2:
		light->getComponent<LightComponent>()->setType(LightComponent::LightType::Spot);
		break;
	default:
		break;
	}

	light->getTransform()->setRotation(Vector3D(m_rotationL[0], m_rotationL[1], m_rotationL[2]));
	light->getTransform()->setPosition(Vector3D(m_x, m_y, m_z));
	light->getComponent<LightComponent>()->setIntensity(m_intensity);
	light->getComponent<LightComponent>()->setColor(Vector3D(m_color[0], m_color[1], m_color[2]));
}