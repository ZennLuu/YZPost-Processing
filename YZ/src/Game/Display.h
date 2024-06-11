#pragma once
#include <Prerequisites.h>
#include <Math/Rect.h>
#include <Window/Window.h>

class Display: public Window
{
public:
	Display(Game* game);
	~Display();

protected:
	virtual void onMove();
	virtual void onSize(const Rect& size);
private:
	SwapChainPtr m_swapChain;
	Game* m_game = nullptr;

	friend class GraphicsEngine;
};

