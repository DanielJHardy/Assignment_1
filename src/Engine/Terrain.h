#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "glm_includes.h"

class Terrain
{
public:
	Terrain();

	void Draw();

	void BuildGrid(vec2 a_worldSize, glm::ivec2 a_gridSize);
	void BuildPerlinTexture(glm::ivec2 a_gridSize);

	void LoadTextures(char* a_grass, char* a_stone);

public:
	float max_height;

	unsigned int m_texture_grass;
	unsigned int m_texture_stone;

private:
	glm::ivec2 m_gridSize;

	bufferData m_mesh;

	unsigned int m_texture_perlin;

};

#endif // !_TERRAIN_H_
