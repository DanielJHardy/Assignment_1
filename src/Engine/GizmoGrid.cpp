#include "GizmoGrid.h"
#include "..\external\Gizmos.h"

GizmoGrid::GizmoGrid()
{
	m_lineColor = vec4(0.1f, 0.1f, 0.1f, 1);
	m_lineColorMid = vec4(1);

	m_size = 10;
}

GizmoGrid::~GizmoGrid(){}

void GizmoGrid::Update(float a_dt)
{
	for (int i = 0; i <= m_size * 2; i++)
	{
		Gizmos::addLine(vec3(-m_size + i, -1, -m_size), vec3(-m_size + i, -1, m_size), i == m_size ? m_lineColorMid : m_lineColor);
		Gizmos::addLine(vec3(-m_size, -1, -m_size + i), vec3(m_size, -1, -m_size + i), i == m_size ? m_lineColorMid : m_lineColor);
	}
}

void GizmoGrid::Draw()
{

}
