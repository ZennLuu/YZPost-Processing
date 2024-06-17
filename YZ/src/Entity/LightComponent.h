#pragma once
#include <Entity/Component.h>
#include <Math/Vector4D.h>

class LightComponent: public Component
{
public:

	enum class LightType
	{
		Directional = 0,
		Point,
		Spot
	};

public:
	LightComponent();
	virtual ~LightComponent();

	void setType(LightType type);
	LightType getType();

	void setColor(const Vector4D& color);
	Vector4D getColor();

	void setIntensity(const float intensity);
	float getIntensity();

	void setRadius(const float radius);
	float getRadius();

protected:
	virtual void onCreateInternal();

private:
	LightType m_type = LightType::Directional;

	Vector4D m_color = Vector4D(1, 1, 1, 1);
	float m_intensity = 1;
	float m_radius = 1;
};