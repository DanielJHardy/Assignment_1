#include "Mesh.h"

#include "..\external\tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\external\stb_image.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "Game.h"

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
		
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
}

void Mesh::LoadOBJ(char* a_filename)
{

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<float> vertex_data;

	//load OBJ file
	tinyobj::LoadObj(shapes, materials, a_filename);

	//set index count
	m_indexCount = shapes[0].mesh.indices.size();

	//read data
	tinyobj::mesh_t* mesh = &shapes[0].mesh;
	vertex_data.reserve(mesh->positions.size() + mesh->normals.size());

	vertex_data.insert(vertex_data.end(), mesh->positions.begin(), mesh->positions.end());
	vertex_data.insert(vertex_data.end(), mesh->normals.begin(), mesh->normals.end());

	//texcoords
	vertex_data.insert(vertex_data.end(), mesh->texcoords.begin(), mesh->texcoords.end());

	//create OpenGL buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->indices.size(), mesh->indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //positions
	glEnableVertexAttribArray(1); //normals
	glEnableVertexAttribArray(2); //texcoords

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)* mesh->positions.size()));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)* (mesh->positions.size() + mesh->normals.size())));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

unsigned int Mesh::LoadTexture(const char* a_filename)
{
	unsigned int rtnTexture;

	int width, height;
	int channels;

	//load file
	unsigned char* data = stbi_load(a_filename, &width, &height, &channels, STBI_default);

	//generate OpenGL texture
	glGenTextures(1, &rtnTexture);
	glBindTexture(GL_TEXTURE_2D, rtnTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//free data
	stbi_image_free(data);

	// return texture handler
	return rtnTexture;
}