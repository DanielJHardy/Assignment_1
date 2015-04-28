#include "Game.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "PointLight.h"
#include "DirectionalLight.h"
#include "ShaderLoading.h"

#include "Callbacks.h"

#define SUCCESS 1;
#define FAILURE 0;

GLFWwindow* Game::m_window;

unsigned int Game::current_shader_program;

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

	//build buffers
	buildGbuffer();
	buildLightBuffer();

	//load shaders
	//gbuffer programs
	LoadShaders("./data/shaders/gbuffer_vertex.glsl","./data/shaders/g_frag_default.glsl",0,&m_g_program_default);
	LoadShaders("./data/shaders/gbuffer_vertex.glsl", "./data/shaders/g_frag_textured.glsl", 0, &m_g_program_diff);

	LoadShaders("./data/shaders/g_vert_terrain.glsl", "./data/shaders/g_frag_terrain.glsl", 0, &m_g_program_terrain);

	//composite
	LoadShaders("./data/shaders/composite_vertex.glsl", "./data/shaders/composite_fragment.glsl", 0, &m_composite_program);

	//light shaders
	LoadShaders("./data/shaders/composite_vertex.glsl", "./data/shaders/directional_light_fragment.glsl", 0, &m_light_directional_program);
	LoadShaders("./data/shaders/point_light_vertex.glsl", "./data/shaders/point_light_fragment.glsl", 0, &m_light_point_program);

	//LEVEL//------------------------//
	m_levels = std::vector<Level*>();

	//set default level
	Level* newLvl = new Level();
	newLvl->m_name = "DEFAULT";

	newLvl->Startup();

	//add default level and set as current level
	m_levels.push_back(newLvl);
	m_levelIndex = 0;
	m_currentLevel = m_levels[0];

	//Lighting
	PointLight::SetupLightMesh();
	DirectionalLight::SetupLightMesh(vec2(m_screenWidth, m_screenHeight));

	//GUI
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(m_screenWidth, m_screenHeight);

	SetCallbacks(m_window);

	m_bar = TwNewBar("Awesome Bar");

	//OpenGL flags
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//setup for deltatime
	glfwSetTime(0.0);

	return true;
}

void Game::Shutdown()
{
	//shutdown the level
	m_currentLevel->Shutdown();

	//gui
	TwDeleteAllBars();
	TwTerminate();

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
	m_currentLevel->Update(dt);

	return true;
}

void Game::Draw()
{
	//enable depth test
	glEnable(GL_DEPTH_TEST);

	/////////////G-BUFFER/////////////////////
	//bind g-buffer and clear
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer_fbo);
	glClearColor(0, 0, 0, 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	vec4 clear_color = vec4(0, 0, 0, 0);
	vec4 clear_normal = vec4(0.5f, 0.5f, 0.5f, 0.5f);

	glClearBufferfv(GL_COLOR, 0, (float*)&clear_color);
	glClearBufferfv(GL_COLOR, 1, (float*)&clear_color);
	glClearBufferfv(GL_COLOR, 2, (float*)&clear_normal);

	//set gbuffer as current shader
	glUseProgram(m_g_program_default); ////////////////////////////////////////////////////////////////////////////////////////////non textured
	current_shader_program = m_g_program_default;

	//set shader uniforms
	int view_uniform = glGetUniformLocation(m_g_program_default, "view");
	int view_proj_uniform = glGetUniformLocation(m_g_program_default, "view_proj");

	glUniformMatrix4fv(view_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getView());	//view
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getProjectionView()); //ProjView

	//////draw scene///////																-<><><>- Objects
	//draw level
	m_currentLevel->Draw_default();

	//set gbuffer as current shader
	glUseProgram(m_g_program_diff);	///////////////////////////////////////////////////////////////////////////////////////////// diffuse textured
	current_shader_program = m_g_program_diff;

	//set shader uniforms
	view_uniform = glGetUniformLocation(m_g_program_diff, "view");
	view_proj_uniform = glGetUniformLocation(m_g_program_diff, "view_proj");

	glUniformMatrix4fv(view_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getView());	//view
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getProjectionView()); //ProjView

	//////draw scene///////																-<><><>- Objects
	//draw level
	m_currentLevel->Draw_diff();

	//set gbuffer as current shader
	glUseProgram(m_g_program_terrain); ////////////////////////////////////////////////////////////////////////////////////////////Terrain
	current_shader_program = m_g_program_terrain;

	//set shader uniforms
	view_uniform = glGetUniformLocation(m_g_program_terrain, "view");
	view_proj_uniform = glGetUniformLocation(m_g_program_terrain, "view_proj");

	glUniformMatrix4fv(view_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getView());	//view
	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getProjectionView()); //ProjView

	int maxHeight_uniform = glGetUniformLocation(m_g_program_terrain, "max_height");
	glUniform1f(maxHeight_uniform, m_currentLevel->m_land.max_height);


	//diffuse uniform
	////set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_currentLevel->m_land.m_texture_grass);

	int grass_uniform = glGetUniformLocation(m_g_program_terrain, "texture_grass");
	glUniform1i(grass_uniform, 0); 

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_currentLevel->m_land.m_texture_stone);

	int stone_uniform = glGetUniformLocation(m_g_program_terrain, "texture_stone");
	glUniform1i(stone_uniform, 1);


	//////draw scene///////																-<><><>- Objects
	//draw level
	m_currentLevel->m_land.Draw();


	//draw Gizmos//////////////////////////////

	///////////////////////<>
	/////////////LIGHT BUFFER/////////////////////

	//bind light buffer and clear
	glBindFramebuffer(GL_FRAMEBUFFER, m_light_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//disable depth test
	glDisable(GL_DEPTH_TEST);

	// enable addative blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	/////Directional Light////
	glUseProgram(m_light_directional_program);

	//set uniforms
	int position_tex_uniform = glGetUniformLocation(m_light_directional_program, "position_tex");
	int normals_tex_uniform = glGetUniformLocation(m_light_directional_program, "normals_tex");

	glUniform1i(position_tex_uniform, 0);
	glUniform1i(normals_tex_uniform, 1);

	//set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_position_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normals_texture);

	//// render directional lights													-<><><>- directional

	//render all the directional lights in the scene
	for each (DirectionalLight light in m_currentLevel->m_lights_directional)
	{
		RenderDirectionalLight(light);
	}

	///
	/////Point Light////

	//use point light shader
	glUseProgram(m_light_point_program);

	//set uniforms
	view_proj_uniform = glGetUniformLocation(m_light_point_program, "proj_view");

	position_tex_uniform = glGetUniformLocation(m_light_point_program, "position_texture");
	normals_tex_uniform = glGetUniformLocation(m_light_point_program, "normal_texture");

	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_currentLevel->m_camera->getProjectionView());
	glUniform1i(position_tex_uniform, 0);
	glUniform1i(normals_tex_uniform, 1);

	//set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_position_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normals_texture);
	//// render point lights														-<><><>- point

	//render every point light in the current scene
	for each (PointLight light in m_currentLevel->m_lights_point)
	{
		RenderPointLight(light);
	}

	///
	//////////////////////////////

	//disable blend
	glDisable(GL_BLEND);

	//unbind buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//clear
	glClearColor(0.3f, 0.3f, 0.3f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////////COMPOSITE PROGRAM///////////////

	//use composite program
	glUseProgram(m_composite_program);

	//set uniforms
	int albedo_tex_uniform = glGetUniformLocation(m_composite_program, "albedo_tex");
	int light_tex_uniform = glGetUniformLocation(m_composite_program, "light_tex");

	glUniform1i(albedo_tex_uniform, 0);
	glUniform1i(light_tex_uniform, 1);

	//set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_albedo_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_light_texture);

	//draw scene quad
	glBindVertexArray(DirectionalLight::light_mesh.m_VAO); //re-using directional light's screenspace-quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//gui
	TwDraw();

	//openGL
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

//Level functions

unsigned int Game::GetCurrentLevel() const
{
	return m_levelIndex;
}

char* Game::GetLevelName(unsigned int a_index) const
{
	return m_levels[a_index]->m_name;
}

void Game::SetLevel(unsigned int a_index)
{
	//check if there is a level with that index
	if (a_index < m_levels.size())
	{
		//if new level starts up
		if (m_levels[a_index]->Startup())
		{
			//shutdown previous level
			m_currentLevel->Shutdown();

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
	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		//set level as current if it has the right name
		if (m_levels[i]->m_name == a_name)
		{
			//if new level starts up
			if (m_levels[i]->Startup())
			{
				//shutdown previous level
				m_currentLevel->Shutdown();

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

unsigned int Game::AddLevel(Level* a_newLvl)
{
	//if there is only the dafault level, replace it
	if (m_levels[0]->m_name == "DEFAULT")
	{
		m_currentLevel->Shutdown();
		m_levels.pop_back();

	}
	else //no need to loop through if there is nothing to loop through - YOLO
	{
		//dont add level if there is already a level with the same name
		for (unsigned int i = 0; i < m_levels.size(); i++)
		{
			if (m_levels[i]->m_name == a_newLvl->m_name)
				return FAILURE;	//return failure
		}
	}

	//add level
	m_levels.push_back(a_newLvl);

	//if this was the first level added
	if (m_levels.size() == 1)
	{
		//set as current level
		m_currentLevel = m_levels[0];

		//startup level
		m_currentLevel->Startup();
	}

	//return success
	return SUCCESS;
}

unsigned int Game::RemoveLevel(unsigned int a_index)
{
	//if level doesnt exist return failure
	if (a_index < m_levels.size())
		return FAILURE;

	//if level is current level
	if (m_levelIndex == a_index)
		return FAILURE;

	//remove level at index
	m_levels.erase(m_levels.begin() + a_index);

	return SUCCESS;
}

unsigned int Game::RemoveLevel(char* a_name)
{
	//restrict from removing current level
	if (m_currentLevel->m_name == a_name)
		return FAILURE;

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		//if level found
		if (m_levels[i]->m_name == a_name)
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

void Game::buildGbuffer()
{
	//framebuffer
	glGenFramebuffers(1, &m_gbuffer_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer_fbo);

	//textures
	//albedo, position, normal, depth()

	//albedo
	glGenTextures(1, &m_albedo_texture);
	glBindTexture(GL_TEXTURE_2D, m_albedo_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_screenWidth, m_screenHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//position
	glGenTextures(1, &m_position_texture);
	glBindTexture(GL_TEXTURE_2D, m_position_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_screenWidth, m_screenHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//normals
	glGenTextures(1, &m_normals_texture);
	glBindTexture(GL_TEXTURE_2D, m_normals_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_screenWidth, m_screenHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//depth
	glGenRenderbuffers(1, &m_gbuffer_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gbuffer_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_screenWidth, m_screenHeight);

	//attach textures to frambuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_albedo_texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_position_texture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_normals_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gbuffer_depth);

	GLenum targets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glDrawBuffers(3, targets);

	//check if it worked
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error in creating gbuffer");
	}

	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::buildLightBuffer()
{
	//create the fbo
	glGenFramebuffers(1, &m_light_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_light_fbo);

	//create textures
	//light texture
	glGenTextures(1, &m_light_texture);
	glBindTexture(GL_TEXTURE_2D, m_light_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_screenWidth, m_screenHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//attach textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_light_texture, 0);

	GLenum light_target = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &light_target);

	//check for success
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error - light framebuffer incorrect");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::RenderDirectionalLight(DirectionalLight a_light)
{
	vec4 viewspace_light_dir = m_currentLevel->m_camera->getView() * vec4(a_light.m_direction, 0);

	int light_dir_uniform = glGetUniformLocation(m_light_directional_program, "light_dir");
	int light_color_uniform = glGetUniformLocation(m_light_directional_program, "light_color");

	glUniform3fv(light_dir_uniform, 1, (float*)&viewspace_light_dir);
	glUniform3fv(light_color_uniform, 1, (float*)&a_light.m_color);

	glBindVertexArray(DirectionalLight::light_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, DirectionalLight::light_mesh.m_indexCount, GL_UNSIGNED_INT, 0);
}

void Game::RenderPointLight(PointLight a_light)
{
	vec4 view_space_pos = m_currentLevel->m_camera->getView() * vec4(a_light.m_pos, 1);

	int pos_uniform = glGetUniformLocation(m_light_point_program, "light_position");
	int view_pos_uniform = glGetUniformLocation(m_light_point_program, "light_view_position");
	int light_diffuse_uniform = glGetUniformLocation(m_light_point_program, "light_diffuse");
	int light_radius_uniform = glGetUniformLocation(m_light_point_program, "light_radius");

	glUniform3fv(pos_uniform, 1, (float*)&a_light.m_pos);
	glUniform3fv(view_pos_uniform, 1, (float*)&view_space_pos);
	glUniform3fv(light_diffuse_uniform, 1, (float*)&a_light.m_color);
	glUniform1f(light_radius_uniform, a_light.m_radius);

	glBindVertexArray(PointLight::light_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, PointLight::light_mesh.m_indexCount, GL_UNSIGNED_INT, 0);
}