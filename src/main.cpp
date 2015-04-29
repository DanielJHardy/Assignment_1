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

void TW_CALL reset_terrain_Callback(void *clientData);

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
	

	Level* enviroment = new Level();

	//level name
	enviroment->m_name = "3D enviroment";

	//terrain
	enviroment->m_land = Terrain();
	enviroment->m_land.BuildGrid();
	enviroment->m_land.BuildPerlinTexture();
	enviroment->m_land.LoadTextures("./data/textures/grass/DIFFUSE.jpg", "./data/textures/rock/DIFFUSE.jpg");

	float above_terrain = enviroment->m_land.max_height + 2;

	//camera
	FlyCam* cam = new FlyCam(glm::radians(60.0f));
	cam->m_speed = 6;

	cam->centerX = (float)game.m_screenWidth / 2;
	cam->centerY = (float)game.m_screenHeight / 2;

	cam->setLookAt(vec3(0, above_terrain, 1), vec3(0, above_terrain, 0), vec3(0, 1, 0));

	enviroment->m_camera = cam;


	//////Actors//////
	Plane* water = new Plane();
	water->Create(vec3(50));
	water->LoadTextures("./data/textures/water/DIFFUSE.jpg",nullptr,nullptr);
	water->SetPosition(vec3(0,-2,0));
	enviroment->AddActor(water);


	Mesh* bob = new Mesh();
	bob->LoadMeshData("./data/models/f16.obj");
	bob->LoadTextures("./data/textures/f16C.bmp", nullptr, nullptr);
	bob->SetPosition(vec3(0, above_terrain, 0));
	enviroment->AddActor(bob);

	


	/////lights///////
	DirectionalLight dirLight = DirectionalLight(vec3(0, -1, 0), vec3(1, 1, 1));
	enviroment->m_lights_directional.push_back(dirLight);


	//GUI
	TwAddButton(game.m_bar, "comment1", NULL, NULL, " label='Terrain Properties' ");
	TwAddSeparator(game.m_bar, "Terrain Properties", "");
	//
	TwAddVarRW(game.m_bar, "World Size", TW_TYPE_DIR3F, &enviroment->m_land.worldSize, "");
	TwAddVarRW(game.m_bar, "Grid Size", TW_TYPE_INT16, &enviroment->m_land.gridSize, "min=32 max=256 step=1");
	TwAddVarRW(game.m_bar, "Height Multiplier", TW_TYPE_FLOAT, &enviroment->m_land.max_height, "min=3 max=20 step=1");
	TwAddVarRW(game.m_bar, "Bumpiness", TW_TYPE_FLOAT, &enviroment->m_land.bumpiness, "min=1 max=15 step=1");
	TwAddVarRW(game.m_bar, "Octaves", TW_TYPE_INT16, &enviroment->m_land.octaves, "min=3 max=12 step=1");
	//
	TwAddSeparator(game.m_bar, "water props", "");
	TwAddButton(game.m_bar, "comment2", NULL, NULL, " label='Water Properties' ");
	//
	TwAddVarRW(game.m_bar, "water_Active", TW_TYPE_INT16, &water->m_active, "label='Active'");
	//
	TwAddSeparator(game.m_bar, "buttons", "");
	//
	TwAddButton(game.m_bar, "rebuildButton", reset_terrain_Callback, &game, " label='Rebuild Terrain' ");

	//add level
	game.AddLevel(enviroment);
}

void TW_CALL reset_terrain_Callback(void *clientData)
{
	Game* game = static_cast<Game*>(clientData);

	//Level* current = game->m_levels[0];
	//current->m_actors_diff[0]->SetPosition(vec3(0,0,0));

	game->m_levels[0]->m_land.BuildGrid();
	game->m_levels[0]->m_land.BuildPerlinTexture();;

}