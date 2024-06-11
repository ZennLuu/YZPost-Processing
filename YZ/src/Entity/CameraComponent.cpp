#include <Entity/CameraComponent.h>
#include <Entity/TransformComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Graphics/GraphicsEngine.h>

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void CameraComponent::getViewMatrix(Matrix4x4& view)
{
	m_entity->getTransform()->getWorldMatrix(view);
	view.inverse();
}

void CameraComponent::getProjectionMatrix(Matrix4x4& proj)
{
	proj = m_projection;
}

void CameraComponent::setFarPlane(f32 farPlane)
{
	m_farPlane = farPlane;
	computeProjectionMatrix();
}

f32 CameraComponent::getFarPlane()
{
	return m_farPlane;
}

void CameraComponent::setNearPlane(f32 nearPlane)
{
	m_nearPlane = nearPlane;
	computeProjectionMatrix();
}

f32 CameraComponent::getNearPlane()
{
	return m_nearPlane;
}

void CameraComponent::setFieldOfView(f32 fieldOfView)
{
	m_fieldOfView = fieldOfView;
	computeProjectionMatrix();
}

f32 CameraComponent::getFieldOfView()
{
	return m_fieldOfView;
}

void CameraComponent::setType(const CameraType& type)
{
	m_type = type;
	computeProjectionMatrix();
}

CameraType CameraComponent::getType()
{
	return m_type;
}

void CameraComponent::setScreenArea(const Rect& area)
{
	m_screenArea = area;
	computeProjectionMatrix();
}

Rect CameraComponent::getScreenArea()
{
	return m_screenArea;
}

void CameraComponent::computeProjectionMatrix()
{
	if (m_type == CameraType::Perspective)
		m_projection.setPerspectiveFovLH(m_fieldOfView, (f32)m_screenArea.width / (f32)m_screenArea.height,
			m_nearPlane, m_farPlane);
	else if (m_type == CameraType::Orthogonal)
		m_projection.setOrthoLH((f32)m_screenArea.width, (f32)m_screenArea.height, m_nearPlane, m_farPlane);
}

void CameraComponent::computeFrustum()
{

}

void CameraComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}