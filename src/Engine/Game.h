#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include "glm_includes.h"

#include "AntTweakBar.h"

#include "Level.h"

struct GLFWwindow;

//Game class for handling low-level game stuff (e.g. state changes, delta time)
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

	//Note: first level added will be set as current level
	unsigned int AddLevel(Level* a_newLvl);				// 
	unsigned int RemoveLevel(unsigned int a_index);		// returns int for success/error
	unsigned int RemoveLevel(char* a_name);				// 


private:
	//Rendering
	void buildGbuffer();
	void buildLightBuffer();

	void RenderDirectionalLight(DirectionalLight a_light);
	void RenderPointLight(PointLight a_light);
	
public: //Variables

	char* m_window_title;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

	//OpenGL
	static GLFWwindow* m_window;

	//shaders
	static unsigned int current_shader_program;

	//GUI
	TwBar* m_bar;
	std::vector<Level*> m_levels;

private:

	// Level
	
	Level* m_currentLevel;
	unsigned int m_levelIndex;

	//shaders
	unsigned int m_composite_program;

	unsigned int m_light_directional_program;
	unsigned int m_light_point_program;

	unsigned int m_g_program_default;
	unsigned int m_g_program_diff;
	unsigned int m_g_program_diff_norm;

	unsigned int m_g_program_terrain;
	unsigned int m_g_program_water;

	//g-buffer
	unsigned int m_gbuffer_fbo;
	unsigned int m_albedo_texture;
	unsigned int m_position_texture;
	unsigned int m_normals_texture;
	unsigned int m_gbuffer_depth;

	//lights
	unsigned int m_light_fbo;
	unsigned int m_light_texture;

};

#endif // !_GAME_H_
