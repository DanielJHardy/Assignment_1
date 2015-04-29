#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "glm_includes.h"
#include <vector>

enum RenderType
{
	RENDER_TYPE_DEFAULT,
	RENDER_TYPE_DIFF,
	RENDER_TYPE_DIFF_NORM
};

class Actor
{
public:
	Actor();
	~Actor();


	//children functions
	void AddChild(Actor* a_child);
	void DestroyChild(unsigned int a_index);
	void Destroy();

	void UpdateFamily(float a_dt);
	void DrawFamily();

	//getter functions
	mat4 getWorldTransform() const;
	mat4 getLocalTransform() const;

	bool getActive() const;

	//transform functions
	void UpdateTransforms();

	void SetPosition(vec3 a_pos);
	void Rotate(float a_angleD, vec3 a_axis);
	void SetScale(vec3 a_scale);

public:

	//should be updated and drawn
	bool m_active;

	RenderType m_renderMode;

protected:
	//transforms
	mat4 m_worldTransform;	//relative to worlds origin
	mat4 m_localTransform;	//relative to parent (which is the same as world if there is no parent)


protected:

	virtual void Update(float a_dt);
	virtual void Draw();

private:
	std::vector<Actor*> m_children;	//parented actors
	Actor* m_parent;

};

#endif // !_ACTOR_H_
