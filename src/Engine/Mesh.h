#ifndef _MESH_H_
#define _MESH_H_

#include "Actor.h"

class Mesh : public Actor
{
public:
	Mesh();
	~Mesh();

	virtual void Update(float a_dt);
	virtual void Draw();

	//loads the mesh data
	void LoadMeshData(char* a_filename);

	// loads the 3 textures for diffuse, normal and specular. I nullptr is
	// input instead of a filename that texture slot will not be used.
	void LoadTextures(char* a_diff, char* a_norm, char* a_spec);

private:
	//obj
	bufferData m_bData;

	//texture
	unsigned int m_texture_diffuse;
	unsigned int m_texture_normal;
	unsigned int m_texture_spec;

};

#endif // !_MESH_H_
