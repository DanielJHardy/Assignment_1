#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "glm_includes.h"
#include <vector>

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

	//should be updated and drawn
	bool m_active;

protected:
	//transforms
	mat4 m_worldTransform;	//relative to worlds origin
	mat4 m_localTransform;	//relative to parent (which is the same as world if there is no parent)

	virtual void Update(float a_dt);
	virtual void Draw();

private:
	std::vector<Actor*> m_children;	//parented actors
	Actor* m_parent;

};

#endif // !_ACTOR_H_
