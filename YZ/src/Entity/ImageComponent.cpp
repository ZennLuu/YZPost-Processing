#include <Entity/ImageComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Resource/Texture.h>
#include <Graphics/Texture2D.h>

ImageComponent::ImageComponent()
{
}

ImageComponent::~ImageComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void ImageComponent::setImage(const TexturePtr& image)
{
	m_image = image;
	m_size = image->getTexture()->getSize();
}

const TexturePtr& ImageComponent::getImage()
{
	return m_image;
}

void ImageComponent::setSize(const Rect& size)
{
	m_size = size;
}

const Rect& ImageComponent::getSize()
{
	return m_size;
}

void ImageComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}
