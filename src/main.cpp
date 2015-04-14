#include "Engine\Game.h"

int main()
{
	Game gameManager;

	//set window properties
	gameManager.m_window_title = "Graphics Assignment";

	gameManager.m_screenWidth = 1280;
	gameManager.m_screenHeight = 720;

	//if game failed to startup
	if (gameManager.Startup() == false)
		return -1;

	//main game loop
	while (gameManager.Update())
	{
		gameManager.Draw();
	}

	return 0;
}