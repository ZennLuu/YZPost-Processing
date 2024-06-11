#include <Entity/LightComponent.h>
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

void LightComponent::setColor(const Vector4D& color)
{
	m_color = color;
}

Vector4D LightComponent::getColor()
{
	return m_color;
}

void LightComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}
