#pragma once
#include <All.h>
#include <Graphics/GraphicsEngine.h>

class Player : public Entity
{
public:
	Player();
	virtual ~Player();

protected:
	virtual void onCreate();
	virtual void onUpdate(f32 deltaTime);
	virtual void onCollision(Component* body1, Component* body2);
private:
	Entity* m_entity = nullptr;
	PlayerControllerComponent* m_player = nullptr;
	CameraComponent* m_camera = nullptr;
	bool m_is_pause = false;

	bool m_onGround = false;
	bool m_isJump = false;

	f32 m_fov = 1.3f;
	f32 m_np = 0.01f;

	f32 m_elapsedSeconds = 0.0f;

	f32 m_forward = 0.0f;
	f32 m_rightward = 0.0f;
	f32 m_upward = 0.0f;
	f32 m_sprint = 1.0f;
};

