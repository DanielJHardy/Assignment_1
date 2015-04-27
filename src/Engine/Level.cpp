#include "Level.h"
#include "..\external\Gizmos.h"

Level::Level()
{
	m_actors_default = std::vector<Actor*>();
	m_actors_diff = std::vector<Actor*>();
	m_actors_diff_norm = std::vector<Actor*>();
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
	for (unsigned int i = 0; i < m_actors_default.size(); i++)
	{
		m_actors_default[i]->UpdateFamily(a_dt);
	}

	for (unsigned int i = 0; i < m_actors_diff.size(); i++)
	{
		m_actors_diff[i]->UpdateFamily(a_dt);
	}

	for (unsigned int i = 0; i < m_actors_diff_norm.size(); i++)
	{
		m_actors_diff_norm[i]->UpdateFamily(a_dt);
	}

	//update camera
	m_camera->Update(a_dt);


	return true;
}

void Level::Draw_default()
{
	//Draw all actors
	for (unsigned int i = 0; i < m_actors_default.size(); i++)
	{
		m_actors_default[i]->DrawFamily();
	}
}

void Level::Draw_diff()
{
	//Draw all actors
	for (unsigned int i = 0; i < m_actors_diff.size(); i++)
	{
		m_actors_diff[i]->DrawFamily();
	}
}

void Level::Draw_diff_norm()
{
	//Draw all actors
	for (unsigned int i = 0; i < m_actors_diff_norm.size(); i++)
	{
		m_actors_diff_norm[i]->DrawFamily();
	}
}

void Level::AddActor(Actor* a_actor)
{
	switch (a_actor->m_renderMode)
	{
	case RENDER_TYPE_DEFAULT:
		m_actors_default.push_back(a_actor);
		break;
	case RENDER_TYPE_DIFF:
		m_actors_diff.push_back(a_actor);
		break;
	case RENDER_TYPE_DIFF_NORM:
		m_actors_diff_norm.push_back(a_actor);
		break;
	default:
		break;
	}
}