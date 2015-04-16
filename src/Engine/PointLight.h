#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_


#include "..\external\gl_core_4_4.h"

#include "glm_includes.h"

class PointLight
{
public:
	PointLight();
	PointLight(const vec3& a_pos, const vec3& a_color, float a_radius);
	~PointLight();

	static void SetupLightMesh();

public:

	vec3 m_pos;
	vec3 m_color;
	float m_radius;

	static bufferData light_mesh;

};


#endif // !_PointLight_H_
