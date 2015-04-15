#ifndef _FLYCAM_H_
#define _FLYCAM_H_

#include "Camera.h"

class FlyCam : public Camera
{
public:
	FlyCam();
	FlyCam(float a_fov);
	~FlyCam();

	virtual void Update(float a_dt);

public:
	float m_speed;			//camera movement speed
	float m_sensitivity;	//mouse sensitivity

	float centerX;
	float centerY;

private:
	vec3 m_up;

	double m_horizontalAngle;
	double m_verticalAngle;

	float m_pitch;
	float m_yaw;
};

#endif // !_FLYCAM_H_
