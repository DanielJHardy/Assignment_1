#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "glm_includes.h"

class Terrain
{
public:
	Terrain();

	void Draw();

	void BuildGrid();
	void BuildPerlinTexture();

	void LoadTextures(char* a_grass, char* a_stone);

public:
	//terrain gen modifiable properties
	float max_height;
	vec3 worldSize;
	int gridSize;
	float bumpiness;
	int octaves;

	//terxtures
	unsigned int m_texture_grass;
	unsigned int m_texture_stone;

private:
	glm::ivec2 m_gridSize;

	bufferData m_mesh;

	unsigned int m_texture_perlin;

	float* perlin_data;

};

#endif // !_TERRAIN_H_
