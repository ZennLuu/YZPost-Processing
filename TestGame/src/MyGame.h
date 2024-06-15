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

private:
	Entity* m_entity = nullptr;
	Entity* m_player = nullptr;
	//UI
	Entity* m_text = nullptr;
	Entity* m_guide = nullptr;
	Entity* m_cross = nullptr;

	Entity* m_skybox = nullptr;


	Entity* m_s1 = nullptr;
	Entity* m_s2 = nullptr;
	Entity* m_s3 = nullptr;



	f32 m_rotation = 0.0f;

	i32 m_fps = 0;
	i32 m_5x = 0;

	PostProcessComponent* m_postProcess = nullptr;

};

