#ifndef _PLANE_H_
#define _PLANE_H_

#include "Actor.h"

class Plane : public Actor
{
public:
	Plane();

	virtual void Update(float a_dt);
	virtual void Draw();

	void Create(vec3 a_scale);
	void LoadTextures(char* a_diff, char* a_norm, char* a_spec);

private:
	bufferData m_bData;

	unsigned int m_texture_diffuse;
	unsigned int m_texture_normal;
	unsigned int m_texture_spec;

};

#endif // !_PLANE_H_
