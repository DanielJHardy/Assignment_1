#ifndef _GLM_INCLUDES_H_
#define _GLM_INCLUDES_H_

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct bufferData
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_indexCount;
};

#endif // !_GLM_INCLUDES_H_
