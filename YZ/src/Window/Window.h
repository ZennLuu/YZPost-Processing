#pragma once
#include <Prerequisites.h>
#include <Math/Rect.h>

class Window
{
public:
	Window();
	virtual ~Window();

	Rect getClientSize();
	Rect getScreenSize();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onMove();
	virtual void onSize(const Rect& size);
protected:
	void* m_hwnd;
	Rect m_size = { 0,0,1024,720 };
};

