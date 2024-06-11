#pragma once
#include <Prerequisites.h>
#include <Math/Rect.h>
#include <SpriteFont.h>

class Font2D
{
public:
	Font2D(const wchar_t* fullPath, RenderSystem* system);
	Rect getBounds(const wchar_t* text);
private:
	std::unique_ptr<DirectX::DX11::SpriteFont> m_font = nullptr;
	std::unique_ptr<DirectX::DX11::SpriteBatch> m_batch = nullptr;

	RenderSystem* m_system = nullptr;

	friend class GraphicsEngine;
};