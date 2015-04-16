#ifndef _SHADERLOADING_H_
#define _SHADERLOADING_H_

#include "glm_includes.h"
#include "..\external\gl_core_4_4.h"

typedef unsigned int GLuint;

bool LoadShaders(char* a_vertex_filename, char* a_fragment_filename, char* a_geometry_filename, GLuint* a_result);
bool LoadShaderType(char* filename, GLenum shader_type, unsigned int* output);

#endif // !_SHADERLOADING_H_
