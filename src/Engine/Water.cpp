#include "Water.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "Game.h"

#include "FileManager.h"

struct terrain_vertex
{
	vec4 position;
	vec2 tex_coord;
};

Water::Water()
{
	m_mesh = bufferData();
	m_time = 0;

	m_texture_diffuse = 0;
	m_texture_displacement = 0;

	m_worldTransform = mat4(1);
}

void Water::Update(float a_dt)
{
	//update running time
	m_time += a_dt;
}

void Water::Draw()
{
	//diffuse uniform
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_diffuse);

	int uniform_location = glGetUniformLocation(Game::current_shader_program, "diffuse");
	glUniform1i(uniform_location, 0);




	glBindVertexArray(m_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_mesh.m_indexCount, GL_UNSIGNED_INT, 0);
}

void Water::Create(vec2 a_size)
{
	//	This function generates the grid we will use for the height map
	//	a_size is the real world dimensions of the grid
	//	gridSize is the number of rows and columns

	int gridSize = a_size.x * 2;

	if (m_mesh.m_indexCount > 0)
	{
		//	first delete any previous buffers if they exist
		glDeleteVertexArrays(1, &m_mesh.m_VAO);
		glDeleteBuffers(1, &m_mesh.m_VBO);
		glDeleteBuffers(1, &m_mesh.m_IBO);
	}
	//	compute how many vertices we need
	unsigned int iVertexCount = (gridSize + 1) * (gridSize + 1);
	//	allocate vertex data
	terrain_vertex*	vertexData = new terrain_vertex[iVertexCount];

	//	compute how many indices we need
	unsigned int iIndexCount = gridSize * gridSize * 6;
	//	allocate index data
	unsigned int* indexData = new unsigned int[iIndexCount];

	//	two nested for loops to generate vertex data
	float fCurrY = -a_size.y * 0.5f;
	for (int y = 0; y < gridSize + 1; ++y)
	{
		float fCurrX = -a_size.x * 0.5f;
		for (int x = 0; x < gridSize + 1; ++x)
		{
			//	inside we create our points, with the grid centred at (0, 0)
			vertexData[y * (gridSize + 1) + x].position = vec4(fCurrX, 0, fCurrY, 1);
			vertexData[y * (gridSize + 1) + x].tex_coord = vec2((float)x / (float)gridSize, (float)y / (float)gridSize);
			fCurrX += a_size.x / (float)gridSize;
		}
		fCurrY += a_size.y / (float)gridSize;
	}

	//	two nested for loops to generate index data
	int	iCurrIndex = 0;
	for (int y = 0; y < gridSize; ++y)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			//	create our 6 indices here!!
			indexData[iCurrIndex++] = y * (gridSize + 1) + x;
			indexData[iCurrIndex++] = (y + 1) * (gridSize + 1) + x;
			indexData[iCurrIndex++] = (y + 1) * (gridSize + 1) + x + 1;

			indexData[iCurrIndex++] = (y + 1) * (gridSize + 1) + x + 1;
			indexData[iCurrIndex++] = y * (gridSize + 1) + x + 1;
			indexData[iCurrIndex++] = y * (gridSize + 1) + x;
		}
	}

	m_mesh.m_indexCount = iIndexCount;

	//	create VertexArrayObject, buffers, etc
	glGenVertexArrays(1, &m_mesh.m_VAO);
	glGenBuffers(1, &m_mesh.m_VBO);
	glGenBuffers(1, &m_mesh.m_IBO);

	//	bind and fill buffers
	glBindVertexArray(m_mesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(terrain_vertex)* iVertexCount, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* iIndexCount, indexData, GL_STATIC_DRAW);

	//	tell OpenGL about our vertex structure
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), (void*)sizeof(vec4));

	//	unbind stuff!!
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//	free vertex and index data
	delete[] vertexData;
	delete[] indexData;
}

void Water::LoadTextures(char* a_diff, char* a_disp)
{
	m_texture_diffuse = LoadTexture(a_diff);
	m_texture_displacement = LoadTexture(a_disp);
}