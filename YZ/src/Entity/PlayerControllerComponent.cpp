#include <Entity/PlayerControllerComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Physics/PhysicsEngine.h>


PlayerControllerComponent::PlayerControllerComponent()
{
}

PlayerControllerComponent::~PlayerControllerComponent()
{
	m_entity->getWorld()->getGame()->getPhysicsEngine()->removeComponent(this);
}

void PlayerControllerComponent::setHeight(f32 height)
{
	m_height = height;
}

f32 PlayerControllerComponent::getHeight()
{
	return m_height;
}

Vector3D PlayerControllerComponent::getMoveDirection()
{
	return m_direction;
}

f32 PlayerControllerComponent::getMoveDistance()
{
	return m_distance;
}

void PlayerControllerComponent::move(const Vector3D& direction, f32 distance)
{
	m_distance = distance;
	m_direction = Vector3D::normalize(direction);
}

void PlayerControllerComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getPhysicsEngine()->addComponent(this);
}