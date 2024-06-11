#pragma once
#include <Entity/Component.h>
#include <Math/Vector4D.h>

class LightComponent: public Component
{
public:
	LightComponent();
	virtual ~LightComponent();

	void setColor(const Vector4D& color);
	Vector4D getColor();

protected:
	virtual void onCreateInternal();

private:
	Vector4D m_color = Vector4D(1, 1, 1, 1);

};

