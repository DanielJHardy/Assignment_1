#include "Game.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

Game::Game()
{

}

//Game flow

bool Game::Startup()
{
	//level
	m_levels = std::vector<Level>();
	m_levelIndex = 0;

	Level newLvl = Level();
}

void Game::Shutdown()
{

}

bool Game::Update()
{

}

void Game::Draw()
{

}

//Level functions

unsigned int Game::GetCurrentLevel() const
{
	return m_levelIndex;
}

char* Game::GetLevelName(unsigned int a_index) const
{
	return m_levels[a_index].name;
}

void Game::SetLevel(unsigned int a_index)
{

}

void Game::SetLevel(char* a_name)
{

}

unsigned int Game::AddLevel(Level a_newLvl)
{

}

unsigned int Game::RemoveLevel(unsigned int a_index)
{

}

unsigned int Game::RemoveLevel(char* a_name)
{

}