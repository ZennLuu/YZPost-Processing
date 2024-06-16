#include <Window/Window.h>
#include <Windows.h>
#include <exception>
#include "imgui/imgui_impl_win32.h"

LRESULT CALLBACK WndProc(HWND hwnd, ui32 msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	//GetWindowLong(hwnd,)
	switch (msg)
	{
	case WM_CREATE:
	{
		// Event fired when the window is created
		// collected here..

		break;
	}
	case WM_MOVE: 
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onMove();
		break;
	}

	case WM_SIZE:
	{
		// Event fired when the window is resized
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onSize(window->getClientSize());
		break;
	}
	case WM_SETFOCUS:
	{
		// Event fired when the window get focus
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		// Event fired when the window lost focus
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		// Event fired when the window is destroyed
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		//::PostQuitMessage(0);
		break;
	}
	case WM_CLOSE: 
	{
		PostQuitMessage(0);
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}




Window::Window()
{
	//Setting up WNDCLASSEX object
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"MyWindowClass";
	wc.lpfnWndProc = &WndProc;

	auto classId = RegisterClassEx(&wc);

	if(!classId) DX3DError("Window not created successfully");

	RECT rc = { 0, 0, m_size.width, m_size.height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"Engine test",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right-rc.left, rc.bottom-rc.top,
		NULL, NULL, NULL, NULL);

	//if the creation fail return false
	if (!m_hwnd) DX3DError("Window not created successfully");

	auto hwnd = static_cast<HWND>(m_hwnd);

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	//show up the window
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	// init ImGui
	ImGui_ImplWin32_Init(hwnd);

	UpdateWindow(hwnd);
}

Rect Window::getClientSize()
{
	RECT rc;
	auto hwnd = static_cast<HWND>(m_hwnd);

	::GetClientRect(hwnd, &rc);
	::ClientToScreen(hwnd, (LPPOINT)&rc.left);
	::ClientToScreen(hwnd, (LPPOINT)&rc.right);
	return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

Rect Window::getScreenSize()
{
	RECT rc = {};

	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);

	return { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

void Window::onMove() 
{
}

void Window::onSize(const Rect& size)
{
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(static_cast<HWND>(m_hwnd));
}
