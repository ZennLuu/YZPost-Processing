#pragma once
#include <Prerequisites.h>
#include <Math/Rect.h>
#include <chrono>

class Game
{
public:
	Game();
	virtual ~Game();

	void run();
	void quit();

	PhysicsEngine* getPhysicsEngine();
	GraphicsEngine* getGraphicsEngine();
	World* getWorld();
	InputSystem* getInputSystem();
	ResourceManager* getResourceManager();
	Display* getDisplay();

	f32 getDeltaTime();

protected:
	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onQuit() {}

	virtual void dramImgui() {}

private:
	void onDisplayMove();
	void onDisplaySize(const Rect& size);
	void onInternalUpdate();
private:
	std::unique_ptr<InputSystem> m_inputSystem;
	std::unique_ptr<PhysicsEngine> m_physicsEngine;
	std::unique_ptr<GraphicsEngine> m_graphicsEngine;
	std::unique_ptr<Display> m_display;
	std::unique_ptr<ResourceManager> m_resourceManager;
	std::unique_ptr<World> m_world;

	bool m_isRunning = true;

	std::chrono::system_clock::time_point m_previousTime;

	f32 m_totalTime = 0.0f;
	f32 m_deltaTime = 0.0f;

	friend class GraphicsEngine;
	friend class Display;
};