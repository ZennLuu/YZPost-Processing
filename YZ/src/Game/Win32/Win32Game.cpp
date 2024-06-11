#include <Game/Game.h>
#include <Window/Window.h>
#include <Windows.h>

void Game::run() 
{
	onCreate();

	MSG msg;
	while (m_isRunning) 
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			if (msg.message == WM_QUIT) 
			{
				m_isRunning = false;
				continue;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		onInternalUpdate();
	}

	onQuit();
}

