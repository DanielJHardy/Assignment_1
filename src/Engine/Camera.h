#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "..\external\gl_core_4_4.h"
#include "glm_includes.h"
#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(float a_fov);
	~Camera();

	virtual void Update(float a_dt);

	void setPerspective(float a_fov, float a_aspectRatio, float a_near, float a_far);
	void setLookAt(vec3 a_from, vec3 a_to, vec3 a_up);
	void setPosition(vec3 a_position);

	mat4 getWorldTransform() const;
	mat4 getView() const;
	mat4 getProjection() const;
	mat4 getProjectionView() const;

	vec4* getFrustrumPlanes(const mat4& a_transform);

protected:
	mat4 m_worldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;

	void updateProjectionViewTransform();

private:
	vec4 m_frustrum[6];

};

#endif // !_CAMERA_H_
