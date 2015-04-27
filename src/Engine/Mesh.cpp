#include "Mesh.h"

#include "..\external\tiny_obj_loader.h"


#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "Game.h"
#include "FileManager.h"

Mesh::Mesh(){}
Mesh::~Mesh(){}

void Mesh::Update(float a_dt)
{
	if (glfwGetKey(Game::m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_localTransform[3] += m_localTransform[0] * 10 * a_dt;
	}
	if (glfwGetKey(Game::m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_localTransform = glm::rotate(m_localTransform, 10.0f * a_dt, vec3(0, 1, 0));
	}
	if (glfwGetKey(Game::m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_localTransform = glm::rotate(m_localTransform, -10.0f * a_dt, vec3(0, 1, 0));
	}
	if (glfwGetKey(Game::m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_localTransform = glm::rotate(m_localTransform, -10.0f * a_dt, vec3(1, 0, 0));
	}

	UpdateTransforms();
}

void Mesh::Draw()
{
	if (m_active)
	{
		//world transform uniform
		int world_uniform = glGetUniformLocation(Game::m_gbuffer_program, "world");
		glUniformMatrix4fv(world_uniform, 1, GL_FALSE, (float*)&m_worldTransform);

		//diffuse uniform
		////set texture slot
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture_diffuse);

		int diffuse_uniform = glGetUniformLocation(Game::m_gbuffer_program, "diffuse");
		glUniform1i(diffuse_uniform, 0); //0 is GL_TEXTURE0
		
		glBindVertexArray(m_bData.m_VAO);
		glDrawElements(GL_TRIANGLES, m_bData.m_indexCount, GL_UNSIGNED_INT, 0);
	}
}

void Mesh::LoadMeshData(char* a_filename)
{
	m_bData = LoadOBJ(a_filename);
}


void Mesh::LoadTextures(char* a_diff, char* a_norm, char* a_spec)
{
	//if has diffuse
	if (a_diff != nullptr)
		m_texture_diffuse = LoadTexture(a_diff);
	else
		m_texture_diffuse = 0;

	//if has normal
	if (a_norm != nullptr)
		m_texture_normal = LoadTexture(a_norm);
	else
		m_texture_normal = 0;

	//if has specular
	if (a_spec != nullptr)
		m_texture_spec = LoadTexture(a_spec);
	else
		m_texture_spec = 0;
}