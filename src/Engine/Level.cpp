#include "Level.h"

Level::Level()
{
	m_actors = std::vector<Actor>();
}

bool Level::Startup()
{
	//make sure m_name is set before startup
	if (m_name == nullptr)
	{
		printf("Level name not set. Set name before startup");
		return false;
	}

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
		m_actors[i].Update();
	}

	return true;
}

void Level::Draw()
{
	//Draw all actors
	for each (Actor actr in m_actors)
	{
		actr.Draw();
	}
}