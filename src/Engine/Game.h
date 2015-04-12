#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

#include "Level.h"

//glm
#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct GLFWwindow;

class Game
{
public:
	Game();

	//Game flow
	bool Startup();
	void Shutdown();

	bool Update();
	void Draw();


	//Level functions
	unsigned int GetCurrentLevel() const;
	char* GetLevelName(unsigned int a_index) const;

	void SetLevel(unsigned int a_index);	//
	void SetLevel(char* a_name);		   //// Sets the current level

	unsigned int AddLevel(Level a_newLvl);				// 
	unsigned int RemoveLevel(unsigned int a_index);		// returns int for success/error
	unsigned int RemoveLevel(char* a_name);				// 


private:

	// Level
	std::vector<Level> m_levels;
	Level m_currentLevel;
	unsigned int m_levelIndex;

	//OpenGL
	GLFWwindow* m_window;
	char* m_window_title;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;


};

#endif // !_GAME_H_