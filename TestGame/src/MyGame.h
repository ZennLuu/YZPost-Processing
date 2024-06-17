#pragma once
#include <All.h>
#include <sstream>

class MyGame: public Game
{
public:
	MyGame();
	~MyGame();

protected:
	virtual void onCreate();
	virtual void onUpdate(f32 deltaTime);

	virtual void dramImgui();

private:
	bool m_toggleimgui = true;
	Entity* m_entity = nullptr;
	Entity* m_player = nullptr;
	//UI
	Entity* m_text = nullptr;
	Entity* m_guide = nullptr;
	Entity* m_cross = nullptr;

	Entity* m_skybox = nullptr;

	Entity* m_light = nullptr;


	Entity* m_s1 = nullptr;
	Entity* m_s2 = nullptr;
	Entity* m_s3 = nullptr;

	MaterialPtr mat1 = nullptr;
	MaterialPtr mat2 = nullptr;
	MaterialPtr mat3 = nullptr;

	float m_x = 0;
	float m_y = 5;
	float m_z = 0;
	float m_intensity = 1.0;
	float m_color[3] = { 1, 1, 1 };
	float m_rotationL[3] = { 0,0,0 };

	float m_speed = 1.0;

	f32 m_rotation = 0.0f;

	i32 m_fps = 0;
	i32 m_5x = 0;

	PostProcessComponent* m_postProcess = nullptr;

};

