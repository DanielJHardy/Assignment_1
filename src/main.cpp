#include "Engine\Game.h"
#include "Engine\FlyCam.h"

//actors
#include "Engine\GizmoGrid.h"

//levels
void AddLevel_3dEnviroment(Game& game);

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

	//Add levels

	////first level automatically set as starting level
	AddLevel_3dEnviroment(gameManager);


	//main game loop
	while (gameManager.Update())
	{
		gameManager.Draw();
	}

	return 0;
}

void AddLevel_3dEnviroment(Game& game)
{
	Level enviroment = Level();

	//level name
	enviroment.m_name = "3D enviroment";

	//camera
	FlyCam* cam = new FlyCam(glm::radians(60.0f));

	cam->centerX = (float)game.m_screenWidth / 2;
	cam->centerY = (float)game.m_screenHeight / 2;

	cam->setLookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0));

	enviroment.m_camera = cam;

	//////Actors//////

	//grid
	GizmoGrid* grid = new GizmoGrid();
	grid->m_size = 100;
	enviroment.m_actors.push_back(grid);


	//add level
	game.AddLevel(enviroment);
}