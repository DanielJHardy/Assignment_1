#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "glm_includes.h"
#include <vector>

class Actor
{
public:
	Actor()
	{
		m_active = true;

		//setup as identity
		m_worldTransform = mat4(1);
		m_localTransform = mat4(1);
	}

	~Actor()
	{
		Destroy();
	}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	//children functions
	void AddChild(Actor* a_child)
	{
		m_children.push_back(a_child);
	}

	void DestroyChild(unsigned int a_index)
	{
		m_children[a_index]->Destroy(); // destroy child's children
		delete m_children[a_index];	//delete child
		m_children.erase(m_children.begin() + a_index); //remove child from children
	}

	void Destroy()
	{
		// for each child
		while (m_children.size() != 0)
		{
			m_children[0]->Destroy(); //call childs destroy
			delete m_children[0];	//delete pointers data
			m_children.erase(m_children.begin());	//remove from list of children
		}
	}

protected:
	//transforms
	mat4 m_worldTransform;	//relative to worlds origin
	mat4 m_localTransform;	//relative to parent (which is the same as world if there is no parent)

	bool m_active;	//should be updated and drawn

private:
	std::vector<Actor*> m_children;	//parented actors


};

#endif // !_ACTOR_H_
