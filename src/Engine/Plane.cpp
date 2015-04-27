#include "Plane.h"

#include "..\external\gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include "Game.h"

struct VertexNormal
{
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float s, t;
};

Plane::Plane()
{

}

void Plane::Update(float a_dt)
{
	if (m_active)
	{
		UpdateTransforms();
	}
}

void Plane::Draw()
{
	if (m_active)
	{
		//world transform uniform
		int world_uniform = glGetUniformLocation(Game::current_shader_program, "world");
		glUniformMatrix4fv(world_uniform, 1, GL_FALSE, (float*)&m_worldTransform);

		glBindVertexArray(m_bData.m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}
}

void Plane::Create(vec3 a_scale)
{
	//Quad buffer creation
	VertexNormal vertexData[] = {
		{ -1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 1, 0, -1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -1, 0, -1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
	};

	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3
	};

	//Vertex Array Object
	glGenVertexArrays(1, &m_bData.m_VAO);
	glBindVertexArray(m_bData.m_VAO);

	//Vertex Buffer Object
	glGenBuffers(1, &m_bData.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_bData.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal) * 4, vertexData, GL_STATIC_DRAW);

	//Element Array Buffer / Index Buffer
	glGenBuffers(1, &m_bData.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bData.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), ((char*)0) + 16);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), ((char*)0) + 48);

	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), ((char*)0) + 32);

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//scale
	m_localTransform[0][0] = a_scale.x;
	m_localTransform[1][1] = a_scale.y;
	m_localTransform[2][2] = a_scale.z;
}