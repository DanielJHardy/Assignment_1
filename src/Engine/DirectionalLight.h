#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#include "..\external\gl_core_4_4.h"

#include "glm_includes.h"

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(const vec3& a_dir, const vec3& a_color);
	~DirectionalLight();

	static void SetupLightMesh(vec2 a_windowSize);

public:
	vec3 m_direction;
	vec3 m_color;

	static bufferData light_mesh;

};



#endif // !_DirectionalLight_H_