#include <Physics/PhysicsEngine.h>

#include <Game/Game.h>

#include <Entity/TerrainComponent.h>
#include <Entity/PlayerControllerComponent.h>
#include <Entity/Entity.h>
#include <Entity/TransformComponent.h>
#include <Entity/MeshComponent.h>

PhysicsEngine::PhysicsEngine(Game* game) :m_game(game)
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::update()
{
	for (auto c : m_components)
	{
		for (auto c1 : m_components)
		{
			if (c == c1) continue;
			auto it = m_checkedComponents.find(c1);
			if (it != m_checkedComponents.end()) continue;

			if (auto cc = dynamic_cast<TerrainComponent*>(c))
			{
				if (auto cc1 = dynamic_cast<PlayerControllerComponent*>(c1)) _processTerrainPlayerCollision(cc, cc1);
				if (auto cc1 = dynamic_cast<MeshComponent*>(c1)) _processTerrainMeshCollision(cc, cc1);
			}
			else if (auto cc = dynamic_cast<PlayerControllerComponent*>(c))
			{
				if (auto cc1 = dynamic_cast<TerrainComponent*>(c1)) _processTerrainPlayerCollision(cc1, cc);
			}
			else if (auto cc = dynamic_cast<MeshComponent*>(c))
			{
				if (auto cc1 = dynamic_cast<TerrainComponent*>(c1)) _processTerrainMeshCollision(cc1, cc);
			}
		}

		m_checkedComponents.emplace(c);
	}

	m_checkedComponents.clear();
}

void PhysicsEngine::addComponent(Component* component)
{
	if (auto c = dynamic_cast<TerrainComponent*>(component))
		m_components.emplace(c);
	else if (auto c = dynamic_cast<PlayerControllerComponent*>(component))
		m_components.emplace(c);
	else if (auto c = dynamic_cast<MeshComponent*>(component))
		m_components.emplace(c);
}

void PhysicsEngine::removeComponent(Component* component)
{
	m_components.erase(component);
}

void PhysicsEngine::_processTerrainPlayerCollision(TerrainComponent* terrain, PlayerControllerComponent* player)
{
	auto playerEntity = player->getEntity();

	Vector3D finalPos;

	if (terrain->intersect(playerEntity->getTransform()->getPosition(), player->getMoveDirection(),
		player->getMoveDistance(), player->getHeight(), finalPos))
	{
		playerEntity->getTransform()->setPosition(finalPos);
		playerEntity->onCollision(player, terrain);
		terrain->getEntity()->onCollision(terrain, player);
	}
}

void PhysicsEngine::_processTerrainMeshCollision(TerrainComponent* terrain, MeshComponent* mesh)
{
}
