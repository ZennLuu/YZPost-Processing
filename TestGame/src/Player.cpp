#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::onCreate()
{
	Entity::onCreate();
	m_camera = createComponent<CameraComponent>();

	m_camera->setFarPlane(10000.0f);
	m_camera->setScreenArea(m_world->getGame()->getDisplay()->getClientSize());
	m_camera->computeFrustum();
	m_player = createComponent<PlayerControllerComponent>();

	getTransform()->setPosition(Vector3D(0, 0, -2));
	getInputSystem()->lockCursor(!m_is_pause);
}

void Player::onUpdate(f32 deltaTime)
{
	Entity::onUpdate(deltaTime);

	auto transform = getTransform();

	m_forward = 0.0f;
	m_rightward = 0.0f;
	m_sprint = 1.0f;

	if (getInputSystem()->isKeyDown(Key::W) && !m_is_pause)
	{
		m_forward = 1.0f;
	}
	if (getInputSystem()->isKeyDown(Key::S) && !m_is_pause)
	{
		m_forward = -1.0f;
	}
	if (getInputSystem()->isKeyDown(Key::A) && !m_is_pause)
	{
		m_rightward = -1.0f;
	}
	if (getInputSystem()->isKeyDown(Key::D) && !m_is_pause)
	{
		m_rightward = 1.0f;
	}
	if(getInputSystem()->isKeyDown(Key::Shift) && !m_is_pause)
	{
		m_sprint = 5.0f;
	}
	//if (getInputSystem()->isKeyDown(Key::Space) && !m_is_pause && !m_isJump)
	//{
	//	m_isJump = true;
	//	m_upward = 0.4f;
	//}

	if (getInputSystem()->isKeyUp(Key::_0))
	{
		m_fov += 0.1f;
		m_camera->setFieldOfView(m_fov);
	}
	if (getInputSystem()->isKeyUp(Key::_9))
	{
		m_fov -= 0.1f;
		m_camera->setFieldOfView(m_fov);
	}
	if(getInputSystem()->isKeyUp(Key::Escape))
	{
		m_is_pause = !m_is_pause;
		getInputSystem()->lockCursor(!m_is_pause);
	}

	auto rot = transform->getRotation();

	if (!m_is_pause) 
	{
		auto deltaPos = getInputSystem()->getDeltaMousePosition();
		rot = rot + Vector3D(deltaPos.y * 0.001f, deltaPos.x * 0.001f, 0);	

		if (rot.x >= 1.57f)
			rot.x = 1.57f;
		else if (rot.x <= -1.57f)
			rot.x = -1.57f;
	}

	Matrix4x4 world;
	transform->getWorldMatrix(world);

	auto pos = transform->getPosition();
	auto dir = Vector3D::normalize(world.getZDirection() * m_forward + world.getXDirection() * m_rightward);
	//pos.y += m_upward;

	pos = pos + dir * 8.0f * m_sprint * deltaTime;

	transform->setPosition(pos);
	transform->setRotation(rot);

	m_onGround = false;
	m_player->move(dir, 8.0f * deltaTime);
	//if(!m_onGround)
	//{
	//	m_upward -= 1.0f * deltaTime;
	//}
}

void Player::onCollision(Component* body1, Component* body2)
{/*
	m_upward = 0.0f;
	m_onGround = true;
	m_isJump = false;*/
}
