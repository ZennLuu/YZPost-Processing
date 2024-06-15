#include "MyGame.h"
#include "Player.h"
#include <time.h>

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

	FontUtility fontUtility;
	fontUtility.createFont(L"Bahnschrift", 16, L"Assets\\Fonts\\");

	auto resourceManager = getResourceManager();

	auto ball = resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/sphere.obj");
	auto suzanna = resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/suzanna.obj");

	auto skyTex = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/sky.jpg");

	auto tex1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/brick_d.jpg");
	auto tex2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/brick_n.jpg");

	auto mtex1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M1/color.jpg");
	auto mtex2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M1/normal.jpg");
	auto mtex3 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M1/metallnes.jpg");
	auto mtex4 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M1/roughness.jpg");

	auto mtex1_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/color.jpg");
	auto mtex2_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/normal.jpg");
	auto mtex3_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/metallnes.jpg");
	auto mtex4_1 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/D/roughness.jpg");

	auto mtex1_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/color.jpg");
	auto mtex2_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/normal.jpg");
	auto mtex3_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/metallnes.jpg");
	auto mtex4_2 = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/M2/roughness.jpg");

	auto cliff = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/ground.jpg");
	auto ground = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/sand.jpg");

	auto waveHeightMap = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/waveHeightMap.png");
	auto heightMap = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/height_map.png");

	auto font = resourceManager->createResourceFromFile<Font>(L"Assets/Fonts/Bahnschrift.font");

	auto cross = resourceManager->createResourceFromFile<Texture>(L"Assets/Textures/UI/cross.png");

	auto skyMat = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/SkyBox.hlsl");
	skyMat->addTexture(skyTex);
	skyMat->setCullMode(CullMode::Front);

	auto mat1 = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/BRDF.hlsl");
	mat1->addTexture(mtex1_2);
	mat1->addTexture(mtex2_2);
	mat1->addTexture(mtex3_2);
	mat1->addTexture(mtex4_2);
	mat1->setCullMode(CullMode::Back);

	auto mat2 = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/BRDF.hlsl");
	mat2->addTexture(mtex1);
	mat2->addTexture(mtex2);
	mat2->addTexture(mtex3);
	mat2->addTexture(mtex4);
	mat2->setCullMode(CullMode::Back);

	auto mat3 = resourceManager->createResourceFromFile<Material>(L"Assets/Shaders/BRDF.hlsl");
	mat3->addTexture(mtex1_1);
	mat3->addTexture(mtex2_1);
	mat3->addTexture(mtex3_1);
	mat3->addTexture(mtex4_1);
	mat3->setCullMode(CullMode::Back);

	//post process
	{
		auto entity = getWorld()->createEntity<Entity>();
		m_postProcess = entity->createComponent<PostProcessComponent>();
		m_postProcess->setShader(L"Assets/Shaders/DefaultPostProcess.hlsl");
	}

	//skybox
	{
		m_skybox = getWorld()->createEntity<Entity>();
		auto meshComp = m_skybox->createComponent<MeshComponent>();
		meshComp->setMesh(ball);
		meshComp->addMaterial(skyMat);
		auto transform = m_skybox->getTransform();
		transform->setScale(Vector3D(1000, 1000, 1000));
	}	
	
	//ball
	{

		m_s1 = getWorld()->createEntity<Entity>();
		auto meshComp = m_s1->createComponent<MeshComponent>();
		meshComp->setMesh(suzanna);
		meshComp->addMaterial(mat1);
		auto transform = m_s1->getTransform();
		transform->setPosition(Vector3D(0, 1, 1));
	}
	
	//ball
	{

		m_s2 = getWorld()->createEntity<Entity>();
		auto meshComp = m_s2->createComponent<MeshComponent>();
		meshComp->setMesh(suzanna);
		meshComp->addMaterial(mat2);
		auto transform = m_s2->getTransform();
		transform->setPosition(Vector3D(3, 1, 1));
	}	
	
	//ball
	{

		m_s3 = getWorld()->createEntity<Entity>();
		auto meshComp = m_s3->createComponent<MeshComponent>();
		meshComp->setMesh(suzanna);
		meshComp->addMaterial(mat3);
		auto transform = m_s3->getTransform();
		transform->setPosition(Vector3D(-3, 1, 1));
	}

	//terrain
	{
		auto entity = getWorld()->createEntity<Entity>();
		auto terrainComp = entity->createComponent<TerrainComponent>();
		terrainComp->setHeightMap(heightMap);
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

	//light
	{
		m_entity = getWorld()->createEntity<Entity>();
		auto lightComponent = m_entity->createComponent<LightComponent>();
		lightComponent->setColor(Vector4D(1.0f, 1.0f, 1.0f, 1));
		lightComponent->setType(LightComponent::LightType::Directional);
		m_entity->getTransform()->setRotation(Vector3D(-0.707f, 0, 0));
		m_entity->getTransform()->setPosition(Vector3D(0, 200, 0));
	}

	//text
	{
		m_text = getWorld()->createEntity<Entity>();
		auto text = m_text->createComponent<TextComponent>();
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
	}

	m_player = getWorld()->createEntity<Player>();
}

void MyGame::onUpdate(f32 deltaTime)
{
	Game::onUpdate(deltaTime);
	m_rotation += 1.57f * deltaTime/ 2;

	m_s1->getTransform()->setRotation(Vector3D(0.0, m_rotation, 0.0));
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

	if (m_5x < 10)
		m_5x++;
	else
		m_5x = 0;

	if (m_5x % 10 == 0)
		m_fps = 1.0f / deltaTime;

	infoText << L"FPS: " << m_fps << L"\n";
	infoText << L"Screen Size W: " << (int)clientSize.width << L" H: " << (int)clientSize.height << L"\n";
	infoText << L"Player coord X: " << (int)playerPos.x << L" Y: " << (int)playerPos.y << L" Z: " << (int)playerPos.z << L"\n";

	textComp1->setText(infoText.str().c_str());

	infoText2 << L"Lock cursor: Escape\n";
	infoText2 << L"Jump: Space\n";
	infoText2 << L"Move: WASD Shift: Accelerate\n";
	infoText2 << L"1 - Default PostProcess, 2 - Chromatic Abberation, 3 - Vignette\n";
	infoText2 << L"4 - PS1, 5 - Grayscale, 6 - Black and White, 7 - HDR\n";

	textComp2->setText(infoText2.str().c_str());
	m_guide->getTransform()->setPosition(Vector3D(
		(f32)(10),
		(f32)(clientSize.height - textBounds2.height - 10),
		0
	));

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

	m_cross->getTransform()->setPosition(Vector3D((f32)(clientSize.width / 2.0f) - 25.0f, (f32)(clientSize.height / 2.0f) - 25.0f, 0));
}
