#include "Actor.h"

Actor::Actor()
{
	m_active = true;

	//setup as identity
	m_worldTransform = mat4(1);
	m_localTransform = mat4(1);

	m_parent = nullptr;

	m_renderMode = RENDER_TYPE_DEFAULT;
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
	a_child->m_parent = this;
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

//Transform functions

void Actor::UpdateTransforms()
{
	if (m_parent == nullptr) //if has parent
	{
		m_worldTransform = m_localTransform;
	}
	else
	{
		m_worldTransform = m_parent->getWorldTransform() * m_localTransform;
	}

	//update childrens transforms aswell
	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateTransforms();
	}
}

void Actor::SetPosition(vec3 a_pos)
{
	m_localTransform[3].xyz = a_pos;
}

void Actor::Rotate(float a_angleD, vec3 a_axis)
{
	m_localTransform = glm::rotate(m_localTransform, a_angleD, a_axis);
}

void Actor::UpdateFamily(float a_dt)
{
	Update(a_dt);

	for (unsigned int i = 0; i < m_children.size(); i++) //recursively update the entire family
	{
		m_children[i]->UpdateFamily(a_dt);
	}
}

void Actor::DrawFamily()
{
	Draw();

	for (unsigned int i = 0; i < m_children.size(); i++) //recursively Draw the entire family
	{
		m_children[i]->DrawFamily();
	}
}
