#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>

#include "AntTweakBar.h"

#include "Actor.h"
#include "Camera.h"

#include "Terrain.h"

#include "DirectionalLight.h"
#include "PointLight.h"

// Level class for containing and updating all actors in the level
class Level
{
public:
	Level();

	bool Startup();
	void Shutdown();

	bool Update(float a_dt);
	void Draw_default();
	void Draw_diff();
	void Draw_diff_norm();

	//actor functions
	void AddActor(Actor* a_actor);	//adds actor to corresponding vector


public:
	Camera* m_camera;

	char* m_name;	//level title

	//actor containers
	std::vector<Actor*> m_actors_default;		//contains all actors with no texture info
	std::vector<Actor*> m_actors_diff;			//contains all actors with a diffuse texture
	std::vector<Actor*> m_actors_diff_norm;		//contains all actors with diffuse and normal textures

	//Lighting

	//light meshes
	bufferData m_pointLight;
	bufferData m_directionalLight;

	//light instances
	std::vector<PointLight> m_lights_point;
	std::vector<DirectionalLight> m_lights_directional;

	//terrain
	Terrain m_land;

};

#endif // !_LEVEL_H_
