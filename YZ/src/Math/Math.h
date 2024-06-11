#pragma once
#include <Prerequisites.h>

class Math
{
public:
	static f32 lerp(f32 a, f32 b, f32 delta)
	{
		return a * (1.0f - delta) + b * (delta);
	}
};