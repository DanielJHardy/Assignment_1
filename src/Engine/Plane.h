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

private:
	bufferData m_bData;

};

#endif // !_PLANE_H_
