#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) 

out vec4 viewspace_position;
out vec4 viewspace_normal;

uniform mat4 view;
uniform mat4 view_proj;
uniform mat4 world;

void main()
{
	viewspace_position = view * world * vec4(position.xyz, 1);
	viewspace_normal = view * world * vec4(normal.xyz, 0);

	gl_Position = view_proj * world * position;
}