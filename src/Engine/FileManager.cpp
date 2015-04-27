#include "FileManager.h"

#include "..\external\tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\external\stb_image.h"

#include "..\external\gl_core_4_4.h"


FileManager* FileManager::m_instance = nullptr;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{
	delete m_instance;
}

bufferData LoadOBJ(char* a_filename)
{
	//If this is the first time a file has been loaded
	if (FileManager::m_instance == nullptr)
		FileManager::m_instance = new FileManager();

	////check if texture has been loaded before
	if (FileManager::m_instance->m_OBJs.count(a_filename) == 1) //if texture has been loaded
	{
		return FileManager::m_instance->m_OBJs[a_filename];
	}

	bufferData rtn_data = bufferData();

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<float> vertex_data;

	//load OBJ file
	std::string err;
	err = tinyobj::LoadObj(shapes, materials, a_filename);

	if (err.size() != 0)
	{
		printf("OBJ load failed: %s", err);
	}
	else
	{

		//set index count
		rtn_data.m_indexCount = shapes[0].mesh.indices.size();

		//read data
		tinyobj::mesh_t* mesh = &shapes[0].mesh;
		vertex_data.reserve(mesh->positions.size() + mesh->normals.size());

		vertex_data.insert(vertex_data.end(), mesh->positions.begin(), mesh->positions.end());
		vertex_data.insert(vertex_data.end(), mesh->normals.begin(), mesh->normals.end());

		//texcoords
		vertex_data.insert(vertex_data.end(), mesh->texcoords.begin(), mesh->texcoords.end());

		//create OpenGL buffers
		glGenVertexArrays(1, &rtn_data.m_VAO);
		glBindVertexArray(rtn_data.m_VAO);

		glGenBuffers(1, &rtn_data.m_VBO);
		glGenBuffers(1, &rtn_data.m_IBO);

		glBindBuffer(GL_ARRAY_BUFFER, rtn_data.m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rtn_data.m_IBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->indices.size(), mesh->indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //positions
		glEnableVertexAttribArray(1); //normals
		glEnableVertexAttribArray(2); //texcoords

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)* mesh->positions.size()));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)* (mesh->positions.size() + mesh->normals.size())));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//store info incase something else wants the same obj
		FileManager::m_instance->m_OBJs.emplace(a_filename, rtn_data);
	}


	//return data
	return rtn_data;
}

unsigned int LoadTexture(char* a_filename)
{
	//If this is the first time a file has been loaded
	if (FileManager::m_instance == nullptr)
		FileManager::m_instance = new FileManager();


	////check if texture has been loaded before
	if (FileManager::m_instance->m_textures.count(a_filename) == 1) //if texture has been loaded
	{
		return FileManager::m_instance->m_textures[a_filename];
	}


	////if file hasnt been loaded yet
	unsigned int rtnTexture;

	int width, height;
	int channels;

	//load file
	unsigned char* data = stbi_load(a_filename, &width, &height, &channels, STBI_default);

	//generate OpenGL texture
	glGenTextures(1, &rtnTexture);
	glBindTexture(GL_TEXTURE_2D, rtnTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//free data
	stbi_image_free(data);


	//store texture incase something else wants to load it
	FileManager::m_instance->m_textures.emplace(a_filename, rtnTexture);


	// return texture handler
	return rtnTexture;
}


