#include <Entity/MeshComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void MeshComponent::setMesh(const MeshPtr& mesh)
{
	m_mesh = mesh;
}

const MeshPtr& MeshComponent::getMesh()
{
	return m_mesh;
}

void MeshComponent::addMaterial(const MaterialPtr& material)
{
	m_materials.push_back(material);
}

void MeshComponent::removeMaterial(ui32 index)
{
	if (index >= m_materials.size()) return;
	m_materials.erase(m_materials.begin() + index);
}

const std::vector<MaterialPtr>& MeshComponent::getMaterials()
{
	return m_materials;
}

void MeshComponent::setGravity(f32 g)
{
	m_gravity = g;
}

f32 MeshComponent::getGravity()
{
	return m_gravity;
}

void MeshComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}
