#include <Resource/Texture.h>
#include <Resource/ResourceManager.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>

Texture::Texture(const wchar_t* full_path, ResourceManager* manager) : Resource(full_path, manager)
{
	m_texture = manager->getGame()->getGraphicsEngine()->getRenderSystem()->createTexture(full_path);
}

Texture::Texture(const Rect& size, Texture2D::Type type, ResourceManager* manager) : Resource(L"", manager)
{
	m_texture = manager->getGame()->getGraphicsEngine()->getRenderSystem()->createTexture(size, type);
}

const Texture2DPtr& Texture::getTexture()
{
	return m_texture;
}
