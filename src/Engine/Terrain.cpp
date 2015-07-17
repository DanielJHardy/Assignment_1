#include "Terrain.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "Game.h"

#include "FileManager.h"

struct terrain_vertex
{
	vec4 position;
	vec2 tex_coord;
	//vec4 normal;
};

Terrain::Terrain()
{
	m_mesh = bufferData();

	//default terrain settings
	max_height = 5;
	worldSize = vec3(100, 100, 0);
	gridSize = 64;
	bumpiness = 1.0f;
	octaves = 6;

	perlin_data = 0;

	m_texture_grass = 0;
	m_texture_stone = 0;
}

void Terrain::Draw()
{

	//grass
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_grass);

	int grass_uniform = glGetUniformLocation(Game::current_shader_program, "texture_grass");
	glUniform1i(grass_uniform, 0);

	//stone
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture_stone);

	int stone_uniform = glGetUniformLocation(Game::current_shader_program, "texture_stone");
	glUniform1i(stone_uniform, 1);

	//perlin
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_texture_perlin);

	int diffuse_uniform = glGetUniformLocation(Game::current_shader_program, "perlin_texture");
	glUniform1i(diffuse_uniform, 3); //0 is GL_TEXTURE0

	glBindVertexArray(m_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_mesh.m_indexCount, GL_UNSIGNED_INT, 0);
}


void Terrain::BuildGrid()
{
	//	This function generates the grid we will use for the height map
	//	worldSize is the real world dimensions of the grid
	//	gridSize is the number of rows and columns

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
	float fCurrY = -worldSize.y * 0.5f;
	for (int y = 0; y < gridSize + 1; ++y)
	{
		float fCurrX = -worldSize.x * 0.5f;
		for (int x = 0; x < gridSize + 1; ++x)
		{
			//	inside we create our points, with the grid centred at (0, 0)

			vertexData[y * (gridSize + 1) + x].position = vec4(fCurrX, 0, fCurrY, 1);
			vertexData[y * (gridSize + 1) + x].tex_coord = vec2((float)x / (float)gridSize, (float)y / (float)gridSize);

			fCurrX += worldSize.x / (float)gridSize;
		}
		fCurrY += worldSize.y / (float)gridSize;
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

	////generate normals
	//for (int i = 0; i < iIndexCount; i += 3)
	//{
	//	vec3 v1 = vertexData[indexData[i]].position.xyz;
	//	vec3 v2 = vertexData[indexData[i + 1]].position.xyz;
	//	vec3 v3 = vertexData[indexData[i + 2]].position.xyz;
	//	vec3 d1 = v2 - v1;
	//	vec3 d2 = v3 - v1;
	//	
	//	vec3 N = glm::cross(d1, d2);
	//	
	//	vertexData[indexData[i]].normal += vec4(N, 1);
	//	vertexData[indexData[i + 1]].normal += vec4(N, 1);
	//	vertexData[indexData[i + 2]].normal += vec4(N, 1);
	//}
	////normalize normals
	//for (int i = 0; i < iVertexCount; i++)
	//{
	//	glm::normalize(vertexData[i].normal);
	//}


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
	//glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), (void*)sizeof(vec4));
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(terrain_vertex), (void*)(sizeof(vec4) + (sizeof(vec2))));

	//	unbind stuff!!
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//	free vertex and index data
	delete[] vertexData;
	delete[] indexData;
}

void Terrain::BuildPerlinTexture()
{
	delete[] perlin_data;

	int dims = gridSize;
	perlin_data = new float[dims * dims]; 
	float scale = (bumpiness / dims) * 3; 

	for (int x = 0; x < dims; ++x)
	{ 
		for (int y = 0; y < dims; ++y)
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims, dims, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void Terrain::LoadTextures(char* a_grass, char* a_stone)
{
	m_texture_grass = LoadTexture(a_grass);
	m_texture_stone = LoadTexture(a_stone);
}
