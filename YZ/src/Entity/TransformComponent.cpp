#include <Entity/TransformComponent.h>

TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::setPosition(const Vector3D& position)
{
	m_position = position; 
	updateWorldMatrix();
}

Vector3D TransformComponent::getPosition()
{
	return m_position;
}

void TransformComponent::setRotation(const Vector3D& rotation)
{
	m_rotation = rotation;
	updateWorldMatrix();
}

Vector3D TransformComponent::getRotation()
{
	return m_rotation;
}

void TransformComponent::setScale(const Vector3D& scale)
{
	m_scale = scale;
	updateWorldMatrix();
}

Vector3D TransformComponent::getScale()
{
	return m_scale;
}

void TransformComponent::getWorldMatrix(Matrix4x4& mat)
{
	mat.setMatrix(m_worldMat);
}

void TransformComponent::updateWorldMatrix()
{
	Matrix4x4 tmp;
	m_worldMat.setIdentity();

	tmp.setIdentity();
	tmp.setScale(m_scale);
	m_worldMat *= tmp;

	tmp.setIdentity();
	tmp.setRotationX(m_rotation.x);
	m_worldMat *= tmp;

	tmp.setIdentity();
	tmp.setRotationY(m_rotation.y);
	m_worldMat *= tmp;
	
	tmp.setIdentity();
	tmp.setRotationZ(m_rotation.z);
	m_worldMat *= tmp;

	tmp.setIdentity();
	tmp.setTranslation(m_position);
	m_worldMat *= tmp;
}
