#pragma once
#include <Entity/Component.h>
#include <Math/Vector3D.h>


class PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent();
	virtual ~PlayerControllerComponent();

	void setHeight(f32 height);
	f32 getHeight();

	Vector3D getMoveDirection();
	f32 getMoveDistance();

	void move(const Vector3D& direction, f32 distance);

protected:
	virtual void onCreateInternal();
private:
	f32 m_height = 1.8f;
	f32 m_distance = 0.0f;
	Vector3D m_direction;
};