#include "Engine\Game.h"
#include "Engine\FlyCam.h"

//actors
#include "Engine\GizmoGrid.h"
#include "Engine\Mesh.h"
#include "Engine\Plane.h"

//lights
#include "Engine\DirectionalLight.h"

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
	cam->m_speed = 6;

	cam->centerX = (float)game.m_screenWidth / 2;
	cam->centerY = (float)game.m_screenHeight / 2;

	cam->setLookAt(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0));

	enviroment.m_camera = cam;

	//////Actors//////
	Mesh* bob = new Mesh();
	bob->LoadMeshData("./data/models/f16.obj");
	bob->LoadTextures("./data/textures/f16C.bmp", nullptr, nullptr);
	//bob->SetPosition(vec3(3,0,-5));
	enviroment.AddActor(bob);

	Plane* phil = new Plane();
	phil->Create(vec3(1));
	enviroment.AddActor(phil);


	/////lights///////
	DirectionalLight dirLight = DirectionalLight(vec3(0, -1, 0), vec3(1, 1, 1));
	enviroment.m_lights_directional.push_back(dirLight);


	//PointLight blueLight = PointLight(vec3(0, 2, 3), vec3(0, 0, 1), 5);
	//enviroment.m_lights_point.push_back(blueLight);


	//add level
	game.AddLevel(enviroment);

}