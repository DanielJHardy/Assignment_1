#include "Mesh.h"

#include "..\external\tiny_obj_loader.h"
#include "..\external\stb_image.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

Mesh::Mesh(){}
Mesh::~Mesh(){}

void Mesh::Update(float a_dt)
{

}

void Mesh::Draw()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
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
	vertex_data.reserve(m_indexCount);

	vertex_data.insert(vertex_data.end(), mesh->positions.begin(), mesh->positions.end());
	vertex_data.insert(vertex_data.end(), mesh->normals.begin(), mesh->normals.end());

	//create OpenGL buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_indexCount, vertex_data.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size()*sizeof(unsigned int), shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //positions
	glEnableVertexAttribArray(1); //normals

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*shapes[0].mesh.positions.size()));

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

unsigned int LoadTexture(const char* a_filename)
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