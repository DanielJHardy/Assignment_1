#include "DirectionalLight.h"

bufferData DirectionalLight::light_mesh = bufferData();

DirectionalLight::DirectionalLight(){}

DirectionalLight::DirectionalLight(const vec3& a_dir, const vec3& a_color)
{
	//set direction
	m_direction = a_dir;

	//set color
	m_color = a_color;
}

DirectionalLight::~DirectionalLight(){}

void DirectionalLight::SetupLightMesh(vec2 a_windowSize)
{
	vec2 halfTexel = 1.0f / (a_windowSize * 0.5f);
	float vertexData[] =
	{
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,
	};
	unsigned int indexData[] = {
		0, 1, 2, 0, 2, 3
	};

	light_mesh.m_indexCount = 6;

	glGenVertexArrays(1, &light_mesh.m_VAO);

	glGenBuffers(1, &light_mesh.m_VBO);
	glGenBuffers(1, &light_mesh.m_IBO);

	glBindVertexArray(light_mesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, light_mesh.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_mesh.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}