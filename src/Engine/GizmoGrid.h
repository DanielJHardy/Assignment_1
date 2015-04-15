#ifndef _GIZMOGRID_H_
#define _GIZMOGRID_H_

#include "Actor.h"

class GizmoGrid : public Actor
{
public:
	GizmoGrid();
	~GizmoGrid();

	virtual void Update(float a_dt);
	virtual void Draw();

public:
	vec4 m_lineColor;
	vec4 m_lineColorMid;

	int m_size;


private:

};

#endif // !_GIZMOGRID_H_
