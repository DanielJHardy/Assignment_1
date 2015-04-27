#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <unordered_map>

#include "bufferData.h"

class FileManager
{
public:
	friend bufferData LoadOBJ(char* a_filename);
	friend unsigned int LoadTexture(char* a_filename);

private:
	FileManager();
	~FileManager();

private:
	static FileManager* m_instance;

	std::unordered_map<char*, unsigned int> m_textures;
	std::unordered_map<char*, bufferData> m_OBJs;

};

#endif // !_FILEMANAGER_H_
