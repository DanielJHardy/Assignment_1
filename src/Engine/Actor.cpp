#include "Actor.h"

Actor::Actor()
{
	m_active = true;

	//setup as identity
	m_worldTransform = mat4(1);
	m_localTransform = mat4(1);
}

Actor::~Actor()
{
	Destroy();
}

void Actor::Update(float a_dt)
{

}

void Actor::Draw()
{

}

//children functions
void Actor::AddChild(Actor* a_child)
{
	m_children.push_back(a_child);
}

void Actor::DestroyChild(unsigned int a_index)
{
	m_children[a_index]->Destroy(); // destroy child's children
	delete m_children[a_index];	//delete child
	m_children.erase(m_children.begin() + a_index); //remove child from children
}

void Actor::Destroy()
{
	// for each child
	while (m_children.size() != 0)
	{
		m_children[0]->Destroy(); //call childs destroy
		delete m_children[0];	//delete pointers data
		m_children.erase(m_children.begin());	//remove from list of children
	}
}

//getter functions

mat4 Actor::getWorldTransform() const
{
	return m_worldTransform;
}

mat4 Actor::getLocalTransform() const
{
	return m_localTransform;
}

bool Actor::getActive() const
{
	return m_active;
}