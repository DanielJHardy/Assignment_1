#include "PointLight.h"

bufferData PointLight::light_mesh = bufferData();

PointLight::PointLight(){}
PointLight::PointLight(const vec3& a_pos, const vec3& a_color, float a_radius)
{
	//set radius
	m_radius = a_radius;

	//set position
	m_pos = a_pos;

	//set color
	m_color = a_color;
}
PointLight::~PointLight(){}

void PointLight::SetupLightMesh()
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

	light_mesh.m_indexCount = 36;

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