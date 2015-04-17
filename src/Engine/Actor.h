#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "glm_includes.h"
#include <vector>

class Actor
{
public:
	Actor();
	~Actor();

	virtual void Update(float a_dt);
	virtual void Draw();

	//children functions
	void AddChild(Actor* a_child);
	void DestroyChild(unsigned int a_index);
	void Destroy();

	//getter functions
	mat4 getWorldTransform() const;
	mat4 getLocalTransform() const;

	bool getActive() const;

	//transform functions
	void SetPosition(vec3 a_pos);

	bool m_active;	//should be updated and drawn

protected:
	//transforms
	mat4 m_worldTransform;	//relative to worlds origin
	mat4 m_localTransform;	//relative to parent (which is the same as world if there is no parent)

private:
	std::vector<Actor*> m_children;	//parented actors

};

#endif // !_ACTOR_H_
