#pragma once
#include <Prerequisites.h>
#include <Entity/Component.h>
#include <Math/Rect.h>
#include <Math/Matrix4x4.h>

struct Plane 
{
	Vector3D normal = Vector3D(0, 1, 0);
	f32 distance = 0.0f;
};

struct Frustum
{
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};


class CameraComponent : public Component
{
public:
	CameraComponent();
	virtual ~CameraComponent();

	void getViewMatrix(Matrix4x4& view);
	void getProjectionMatrix(Matrix4x4& proj);


	void setFarPlane(f32 farPlane);
	f32 getFarPlane();

	void setNearPlane(f32 nearPlane);
	f32 getNearPlane();

	void setFieldOfView(f32 fieldOfView);
	f32 getFieldOfView();

	void setType(const CameraType& type);
	CameraType getType();

	void setScreenArea(const Rect& area);
	Rect getScreenArea();

	void computeFrustum();

private:
	void computeProjectionMatrix();
protected:
	virtual void onCreateInternal();
private:
	Matrix4x4 m_projection;

	f32 m_nearPlane = 0.01f;
	f32 m_farPlane = 100.0f;
	f32 m_fieldOfView = 1.3f;
	CameraType m_type = CameraType::Perspective;
	Rect m_screenArea;
};