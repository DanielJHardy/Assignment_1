#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>

#include "Actor.h"
#include "Camera.h"
#include "Light.h"

// Level class for containing and updating all actors in the level
class Level
{
public:
	Level();

	bool Startup();
	void Shutdown();

	bool Update(float a_dt);
	void Draw();

public:
	std::vector<Actor*> m_actors;	//all the actors in the level
	Camera* m_camera;

	char* m_name;	//level title

	//Lighting

	//light meshes
	bufferData m_pointLight;
	bufferData m_directionalLight;

	//light instances
	std::vector<Light> m_lights

};

#endif // !_LEVEL_H_
