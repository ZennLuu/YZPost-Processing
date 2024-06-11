#include <Game/Display.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>
#include <Graphics/RenderSystem.h>
#include <Graphics/SwapChain.h>

Display::Display(Game* game): m_game(game)
{
	auto size = getClientSize();
	m_swapChain = game->getGraphicsEngine()->getRenderSystem()->createSwapChain(static_cast<HWND>(m_hwnd), size.width, size.height);
}

Display::~Display()
{
}

void Display::onMove()
{
	m_game->onDisplayMove();
}

void Display::onSize(const Rect& size)
{
	m_swapChain->resize(size.width,size.height);
	m_game->onDisplaySize(size);
	m_game->getGraphicsEngine()->resizePostProcess(size);
}
