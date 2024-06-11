#pragma once
#include <Prerequisites.h>
#include <set>


class PhysicsEngine
{
public:
	PhysicsEngine(Game* game);
	virtual ~PhysicsEngine();

	void update();
public:
	void addComponent(Component* component);
	void removeComponent(Component* component);
private:
	void _processTerrainPlayerCollision(TerrainComponent* terrain, PlayerControllerComponent* player);
	void _processTerrainMeshCollision(TerrainComponent* terrain, MeshComponent* mesh);
private:
	Game* m_game = nullptr;

	std::set<Component*> m_components;
	std::set<Component*> m_checkedComponents;
};