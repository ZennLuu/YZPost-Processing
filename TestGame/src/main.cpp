#include "MyGame.h"

int main()
{
	try
	{
		MyGame game;
		game.run();
	}
	catch (...) 
	{
		return -1;
	}
}