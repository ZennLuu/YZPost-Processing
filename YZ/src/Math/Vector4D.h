#pragma once
#include <Math/Vector3D.h>

class Vector4D
{
public:
	Vector4D() :x(0), y(0), z(0),m_w(0)
	{
	}
	Vector4D(f32 x, f32 y, f32 z,f32 w) :x(x), y(y), z(z), m_w(w)
	{
	}
	Vector4D(const Vector4D& vector) :x(vector.x), y(vector.y), z(vector.z), m_w(vector.m_w)
	{
	}
	Vector4D(const Vector3D& vector) :x(vector.x), y(vector.y), z(vector.z), m_w(1.0)
	{
	}

	void cross(Vector4D &v1, Vector4D &v2, Vector4D &v3)
	{
		this->x = v1.y * (v2.z * v3.m_w - v3.z * v2.m_w) - v1.z * (v2.y * v3.m_w - v3.y * v2.m_w) + v1.m_w * (v2.y * v3.z - v2.z *v3.y);
		this->y = -(v1.x * (v2.z * v3.m_w - v3.z * v2.m_w) - v1.z * (v2.x * v3.m_w - v3.x * v2.m_w) + v1.m_w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.m_w - v3.y * v2.m_w) - v1.y * (v2.x *v3.m_w - v3.x * v2.m_w) + v1.m_w * (v2.x * v3.y - v3.x * v2.y);
		this->m_w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x *v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

	~Vector4D()
	{
	}

public:
	f32 x, y, z,m_w;
};