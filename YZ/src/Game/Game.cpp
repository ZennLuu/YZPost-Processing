#include <Game/Game.h>
#include <Window/Window.h>
#include <Graphics/GraphicsEngine.h>
#include <Game/Display.h>
#include <Game/World.h>
#include <Resource/ResourceManager.h>
#include <Resource/Mesh.h>
#include <Resource/Texture.h>
#include <Resource/Material.h>
#include <Input/InputSystem.h>
#include <Physics/PhysicsEngine.h>


Game::Game()
{
	m_inputSystem = std::make_unique<InputSystem>();
	m_physicsEngine = std::make_unique<PhysicsEngine>(this);
	m_graphicsEngine = std::make_unique<GraphicsEngine>(this);
	m_display = std::make_unique<Display>(this);
	m_resourceManager = std::make_unique<ResourceManager>(this);
	m_world = std::make_unique<World>(this);

	this->getGraphicsEngine()->createPostProcessInternal();

	m_inputSystem->setLockArea(m_display->getClientSize());
//	m_inputSystem->lockCursor(true);
}

Game::~Game()
{
}

PhysicsEngine* Game::getPhysicsEngine()
{
	return m_physicsEngine.get();
}

GraphicsEngine* Game::getGraphicsEngine()
{
	return m_graphicsEngine.get();
}

World* Game::getWorld()
{
	return m_world.get();
}

InputSystem* Game::getInputSystem()
{
	return m_inputSystem.get();
}

ResourceManager* Game::getResourceManager()
{
	return m_resourceManager.get();
}

Display* Game::getDisplay()
{
	return m_display.get();
}

f32 Game::getDeltaTime()
{
	return m_deltaTime;
}

void Game::onDisplayMove()
{
	m_inputSystem->setLockArea(m_display->getClientSize());
	onInternalUpdate();
}

void Game::onDisplaySize(const Rect& size)
{
	m_inputSystem->setLockArea(m_display->getClientSize());
	onInternalUpdate();
}

void Game::onInternalUpdate()
{
	//computing delta Time
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();
	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;
	m_previousTime = currentTime;

	m_deltaTime = (f32)elapsedSeconds.count();
	m_totalTime += m_deltaTime;

	m_inputSystem->update();

	onUpdate(m_deltaTime);
	m_world->update(m_deltaTime);

	m_physicsEngine->update();
	m_graphicsEngine->update();
}

void Game::quit()
{
	m_isRunning = false;
}