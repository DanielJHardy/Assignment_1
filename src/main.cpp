#include "Engine\Game.h"

int main()
{
	Game gameManager;

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