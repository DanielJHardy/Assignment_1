#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>
#include "Actor.h"
#include "Camera.h"

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

};

#endif // !_LEVEL_H_
