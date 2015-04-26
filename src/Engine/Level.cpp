#include "Level.h"
#include "..\external\Gizmos.h"

Level::Level()
{
	m_actors = std::vector<Actor*>();
}

bool Level::Startup()
{
	//make sure m_name is set before startup
	if (m_name == nullptr)
	{
		printf("Level name not set. Set name before startup");
		return false;
	}

	//setup gizmos
	Gizmos::create();

	Gizmos::addTransform(mat4(1), 1);

	return true;
}

void Level::Shutdown()
{

}

bool Level::Update(float a_dt)
{
	//Update all the actors
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		m_actors[i]->UpdateFamily(a_dt);
	}

	//update camera
	m_camera->Update(a_dt);


	return true;
}

void Level::Draw()
{
	//Draw all actors
	for (unsigned int i = 0; i < m_actors.size(); i++)
	{
		m_actors[i]->DrawFamily();
	}

	Gizmos::draw(m_camera->getProjectionView());
}