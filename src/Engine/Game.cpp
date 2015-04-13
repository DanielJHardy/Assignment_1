#include "Game.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

#define SUCCESS 1;
#define FAILURE 0;

class bob : public Level
{

};

Game::Game()
{

}

//Game flow

bool Game::Startup()
{
	//OPENGL//-----------------------//

	// Loads up glfw
	if (glfwInit() == 0)
		return false;

	//creates a window
	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, m_window_title, nullptr, nullptr);

	// Check for window creation failure
	if (m_window == nullptr)
		return false;

	//sets window as current
	glfwMakeContextCurrent(m_window);

	//load opengl
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	int major_version = ogl_GetMajorVersion();
	int minor_version = ogl_GetMinorVersion();

	printf("Successfully loaded OpenGL version %d.%d\n", major_version, minor_version);


	//LEVEL//------------------------//
	m_levels = std::vector<Level>();

	//set default level
	Level newLvl = Level();
	newLvl.m_name = "DEFAULT";

	newLvl.Startup();

	//add default level and set as current level
	m_levels.push_back(bob());

	m_levels.push_back(newLvl);
	m_levelIndex = 0;
	m_currentLevel = m_levels[0];

}

void Game::Shutdown()
{
	//shutdown the level
	m_currentLevel.Shutdown();

	//openGL
	glfwDestroyWindow(this->m_window);
	glfwTerminate();
}

bool Game::Update()
{
	//check if game should close
	if (glfwWindowShouldClose(this->m_window))
		return false;

	//Get delta time
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);

	//update level
	m_currentLevel.Update(dt);

	return true;
}

void Game::Draw()
{
	//draw level
	m_currentLevel.Draw();
}

//Level functions

unsigned int Game::GetCurrentLevel() const
{
	return m_levelIndex;
}

char* Game::GetLevelName(unsigned int a_index) const
{
	return m_levels[a_index].m_name;
}

void Game::SetLevel(unsigned int a_index)
{
	//check if there is a level with that index
	if (a_index < m_levels.size)
	{
		//if new level starts up
		if (m_levels[a_index].Startup())
		{
			//shutdown previous level
			m_currentLevel.Shutdown();

			//start new level
			m_levelIndex = a_index;
			m_currentLevel = m_levels[a_index];
		}
		else // failed to startup
		{
			printf("failed: set level. level wouldnt startup");
		}
	}
}

void Game::SetLevel(char* a_name)
{
	// look for level with the right name
	for (unsigned int i = 0; i < m_levels.size; i++)
	{
		//set level as current if it has the right name
		if (m_levels[i].m_name == a_name)
		{
			//if new level starts up
			if (m_levels[i].Startup())
			{
				//shutdown previous level
				m_currentLevel.Shutdown();

				//start new level
				m_levelIndex = i;
				m_currentLevel = m_levels[i];
			}
			else //failed to startup
			{
				printf("failed: set level. level wouldnt startup");
			}
			return;
		}
	}

	//level not found
	printf("level name doest exist. level not set");
}

unsigned int Game::AddLevel(Level a_newLvl)
{
	//if there is only the dafault level, replace it
	if (m_levels[0].m_name == "DEFAULT")
	{
		m_currentLevel.Shutdown();
		m_levels.pop_back();

	}
	else //no need to loop through if there is nothing to loop through - YOLO
	{
		//dont add level if there is already a level with the same name
		for (unsigned int i = 0; i < m_levels.size; i++)
		{
			if (m_levels[i].m_name == a_newLvl.m_name)
				return FAILURE;	//return failure
		}
	}

	//add level
	m_levels.push_back(a_newLvl);

	//if this was the first level added
	if (m_levels.size == 1)
	{
		//set as current level
		m_currentLevel = m_levels[0];

		//startup level
		m_currentLevel.Startup();
	}

	//return success
	return SUCCESS;
}

unsigned int Game::RemoveLevel(unsigned int a_index)
{
	//if level doesnt exist return failure
	if (a_index < m_levels.size)
		return FAILURE;

	//if level is current level
	if (m_levelIndex == a_index)
		return FAILURE;

	//remove level at index
	m_levels.erase(m_levels.begin + a_index);

	return SUCCESS;
}

unsigned int Game::RemoveLevel(char* a_name)
{
	//restrict from removing current level
	if (m_currentLevel.m_name == a_name)
		return FAILURE;

	for (unsigned int i = 0; i < m_levels.size; i++)
	{
		//if level found
		if (m_levels[i].m_name == a_name)
		{
			//set as current
			m_levelIndex = i;
			m_currentLevel = m_levels[i];

			return SUCCESS;
		}
	}

	//if level wasnt found
	return FAILURE;
}