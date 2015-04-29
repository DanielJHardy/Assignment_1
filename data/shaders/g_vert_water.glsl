#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;  

uniform mat4 view;
uniform mat4 view_proj;

out vec4 viewspace_position;
out vec4 viewspace_normal;
out vec2 vTexCoord;  

void main()
{
	vTexCoord = texcoord;  

	viewspace_position = view * vec4(position.xyz, 1);
	viewspace_normal = view * vec4(0, 1, 0, 0); //not using atm

	gl_Position = view_proj * position;
}