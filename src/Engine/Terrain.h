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

private:
	glm::ivec2 m_gridSize;

	bufferData m_mesh;

	unsigned int m_texture_perlin;

};

#endif // !_TERRAIN_H_
