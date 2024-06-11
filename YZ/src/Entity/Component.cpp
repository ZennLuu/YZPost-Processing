#include <Entity/Component.h>
#include <Entity/Entity.h>

Component::Component()
{
}

Component::~Component()
{
}

void Component::release()
{
	m_entity->removeComponent(m_typeId);
}

Entity* Component::getEntity()
{
	return m_entity;
}

void Component::onCreateInternal()
{
}
