#include <Entity/TextComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/Font2D.h>
#include <Resource/Font.h>

TextComponent::TextComponent()
{
}

TextComponent::~TextComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void TextComponent::setFont(const FontPtr& font)
{
	m_font = font;
}

const FontPtr& TextComponent::getFont()
{
	return m_font;
}

void TextComponent::setText(const wchar_t* text)
{
	m_text = text;
}

const wchar_t* TextComponent::getText()
{
	return m_text.c_str();
}

Rect TextComponent::getBounds()
{
	return m_font->getFont()->getBounds(m_text.c_str());
}

void TextComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}