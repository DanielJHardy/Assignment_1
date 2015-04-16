#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_


#include "..\external\gl_core_4_4.h"

#include "glm_includes.h"

class PointLight
{
public:
	PointLight(){}
	PointLight(const vec3& a_pos, const vec3& a_color, float a_radius)
	{
		//set radius
		m_radius = a_radius;

		//set position
		m_pos = a_pos;

		//set color
		m_color = a_color;
	}
	~PointLight(){}

	static void SetupLightMesh(vec2 a_windowSize);

public:

	vec3 m_pos;
	vec3 m_color;
	float m_radius;

	static bufferData light_mesh;

};

void PointLight::SetupLightMesh(vec2 a_windowSize)
{
	float vertexData[] =
	{
		//bottom
		-1, -1, 1, 1,
		1, -1, 1, 1,
		1, -1, -1, 1,
		-1, -1, -1, 1,

		//top
		-1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, -1, 1,
		-1, 1, -1, 1,
	};
	unsigned int indexData[] = {
		4, 5, 0,
		5, 1, 0,
		5, 6, 1,
		6, 2, 1,
		6, 7, 2,
		7, 3, 2,
		7, 4, 3,
		4, 0, 3,
		7, 6, 4,
		6, 5, 4,
		0, 1, 3,
		1, 2, 3

	};

	glGenVertexArrays(1, &light_mesh.m_VAO);

	glGenBuffers(1, &light_mesh.m_VBO);
	glGenBuffers(1, &light_mesh.m_IBO);

	glBindVertexArray(light_mesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, light_mesh.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_mesh.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif // !_PointLight_H_
