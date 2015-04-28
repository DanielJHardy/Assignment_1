#include "Terrain.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "Game.h"

struct terrain_vertex
{
	vec4 position;
	vec2 tex_coord;
};

Terrain::Terrain()
{
	m_mesh = bufferData();
}

void Terrain::Draw()
{
	//diffuse uniform
	////set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_perlin);

	int diffuse_uniform = glGetUniformLocation(Game::current_shader_program, "perlin_texture");
	glUniform1i(diffuse_uniform, 0); //0 is GL_TEXTURE0

	glBindVertexArray(m_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_mesh.m_indexCount, GL_UNSIGNED_INT, 0);
}


void Terrain::BuildGrid(vec2 a_worldSize, glm::ivec2 a_gridSize)
{
	//	This function generates the grid we will use for the height map
	//	a_worldSize is the real world dimensions of the grid
	//	a_gridSize is the number of rows and columns

	if (m_mesh.m_indexCount > 0)
	{
		//	first delete any previous buffers if they exist
		glDeleteVertexArrays(1, &m_mesh.m_VAO);
		glDeleteBuffers(1, &m_mesh.m_VBO);
		glDeleteBuffers(1, &m_mesh.m_IBO);
	}
	//	compute how many vertices we need
	unsigned int iVertexCount = (a_gridSize.x + 1) * (a_gridSize.y + 1);
	//	allocate vertex data
	terrain_vertex*	vertexData = new terrain_vertex[iVertexCount];

	//	compute how many indices we need
	unsigned int iIndexCount = a_gridSize.x * a_gridSize.y * 6;
	//	allocate index data
	unsigned int* indexData = new unsigned int[iIndexCount];

	//	two nested for loops to generate vertex data
	float fCurrY = -a_worldSize.y * 0.5f;
	for (int y = 0; y < a_gridSize.y + 1; ++y)
	{
		float fCurrX = -a_worldSize.x * 0.5f;
		for (int x = 0; x < a_gridSize.x + 1; ++x)
		{
			//	inside we create our points, with the grid centred at (0, 0)
			vertexData[y * (a_gridSize.x + 1) + x].position = vec4(fCurrX, 0, fCurrY, 1);
			vertexData[y * (a_gridSize.x + 1) + x].tex_coord = vec2((float)x / (float)a_gridSize.x, (float)y / (float)a_gridSize.y);
			fCurrX += a_worldSize.x / (float)a_gridSize.x;
		}
		fCurrY += a_worldSize.y / (float)a_gridSize.y;
	}

	//	two nested for loops to generate index data
	int	iCurrIndex = 0;
	for (int y = 0; y < a_gridSize.y; ++y)
	{
		for (int x = 0; x < a_gridSize.x; ++x)
		{
			//	create our 6 indices here!!
			indexData[iCurrIndex++] = y * (a_gridSize.x + 1) + x;
			indexData[iCurrIndex++] = (y + 1) * (a_gridSize.x + 1) + x;
			indexData[iCurrIndex++] = (y + 1) * (a_gridSize.x + 1) + x + 1;

			indexData[iCurrIndex++] = (y + 1) * (a_gridSize.x + 1) + x + 1;
			indexData[iCurrIndex++] = y * (a_gridSize.x + 1) + x + 1;
			indexData[iCurrIndex++] = y * (a_gridSize.x + 1) + x;
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

void Terrain::BuildPerlinTexture(glm::ivec2 a_gridSize)
{

	//float* perlin_data = new float[a_gridSize.x * a_gridSize.y];
	
	int dims = a_gridSize.x; 
	float *perlin_data = new float[dims * dims]; 
	float scale = (1.0f / dims) * 3; 
	int octaves = 6;

	for (int x = 0; x < 64; ++x) 
	{ 
		for (int y = 0; y < 64; ++y)     
		{ 
			float amplitude = 1.0f;
			float persistence = 0.3f;
			perlin_data[y* dims + x] = glm::perlin(vec2(x, y) * scale) * 0.5f + 0.5f; 

			for (int o = 0; o < octaves; ++o)  
			{ 
				float freq = powf(2, (float)o);      
				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;      
				perlin_data[y * dims + x] += perlin_sample * amplitude;      
				amplitude *= persistence; 
			}
		} 
	}

	//gen texture
	glGenTextures(1, &m_texture_perlin);
	
	glBindTexture(GL_TEXTURE_2D, m_texture_perlin);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}