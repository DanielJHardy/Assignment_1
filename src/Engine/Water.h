#ifndef _WATER_H_
#define _WATER_H_

#include "glm_includes.h"

class Water
{
public:
	Water();

	void Update(float a_dt);
	void Draw();

	void Create(vec2 a_size);

	void LoadTextures(char* a_diff, char* a_disp);

	////////////////////////////
	mat4 m_worldTransform;

	bool m_active;

private:
	bufferData m_mesh;

	//textures
	unsigned int m_texture_diffuse;
	unsigned int m_texture_displacement;

	float m_time;

};

#endif // !_WATER_H_
