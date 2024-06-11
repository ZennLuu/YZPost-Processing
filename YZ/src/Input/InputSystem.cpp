#include <Input/InputSystem.h>
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

bool InputSystem::isKeyDown(const Key& key)
{
	return (m_final_keys_state[getInternalKeyCode(key)] == 0);
}

bool InputSystem::isKeyUp(const Key& key)
{
	return (m_final_keys_state[getInternalKeyCode(key)] == 1);
}

Vector2D InputSystem::getDeltaMousePosition()
{
	return m_deltaMousePos;
}

void InputSystem::lockCursor(bool lock)
{
	m_cursorLocked = lock;
	if (lock) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) <= 1);
}

void InputSystem::setLockArea(const Rect& area)
{
	m_lockArea = area;
	m_lockAreaCenter = Vector2D(floor(area.left + (float)area.width / 2.0f), floor(area.top + (float)area.height / 2.0f));
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);


	if (current_mouse_pos.x != m_old_mouse_pos.x || current_mouse_pos.y != m_old_mouse_pos.y)
	{
		m_deltaMousePos = Vector2D((float)current_mouse_pos.x - (float)m_old_mouse_pos.x,
			(float)current_mouse_pos.y - (float)m_old_mouse_pos.y);
	}
	else
	{
		m_deltaMousePos = Vector2D(0, 0);
	}


	if (!m_cursorLocked)
		m_old_mouse_pos = Vector2D((float)current_mouse_pos.x, (float)current_mouse_pos.y);
	else
	{
		SetCursorPos((int)m_lockAreaCenter.x, (int)m_lockAreaCenter.y);
		m_old_mouse_pos = m_lockAreaCenter;
	}



	for (ui32 i = 0; i < 256; i++)
	{
		m_keys_state[i] = ::GetAsyncKeyState(i);
		// KEY IS DOWN
		if (m_keys_state[i] & 0x8001)
		{
			m_final_keys_state[i] = 0;
		}
		else // KEY IS UP
		{
			if (m_keys_state[i] != m_old_keys_state[i])
			{
				m_final_keys_state[i] = 1;
			}
			else
			{
				m_final_keys_state[i] = 2;
			}

		}

	}
	// store current keys state to old keys state buffer
	::memcpy(m_old_keys_state, m_keys_state, sizeof(short) * 256);
}

short InputSystem::getInternalKeyCode(const Key& key)
{
	short keyWin = 0;

	if (key >= Key::A && key <= Key::Z)
		keyWin = 'A' + ((short)key - (short)Key::A);
	else if (key >= Key::_0 && key <= Key::_9)
		keyWin = '0' + ((short)key - (short)Key::_0);
	else if (key == Key::Shift)
		keyWin = VK_SHIFT;
	else if (key == Key::Escape)
		keyWin = VK_ESCAPE;
	else if (key == Key::Space)
		keyWin = VK_SPACE;
	else if (key == Key::Enter)
		keyWin = VK_RETURN;
	else if (key == Key::LeftMouseButton)
		keyWin = VK_LBUTTON;
	else if (key == Key::MiddleMouseButton)
		keyWin = VK_MBUTTON;
	else if (key == Key::RightMouseButton)
		keyWin = VK_RBUTTON;

	return keyWin;
}