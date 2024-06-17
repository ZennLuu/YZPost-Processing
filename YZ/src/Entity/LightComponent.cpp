#include <Entity/LightComponent.h>
#include <Entity/TransformComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>

LightComponent::LightComponent()
{
}

LightComponent::~LightComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void LightComponent::setType(LightType type)
{
	m_type = type;
}

LightComponent::LightType LightComponent::getType()
{
	return m_type;
}

void LightComponent::setColor(const Vector4D& color)
{
	m_color = color;
}

Vector4D LightComponent::getColor()
{
	return m_color;
}

void LightComponent::setIntensity(const float intensity)
{
	m_intensity = intensity;
}

float LightComponent::getIntensity()
{
	return m_intensity;
}

void LightComponent::setRadius(const float radius)
{
	m_radius = radius;
}

float LightComponent::getRadius()
{
	return m_radius;
}

void LightComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}