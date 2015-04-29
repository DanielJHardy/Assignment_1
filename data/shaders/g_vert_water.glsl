#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;

out vec4 viewspace_position;
out vec4 viewspace_normal;

uniform mat4 view;
uniform mat4 view_proj;

uniform float Time;

uniform sampler2D displacement;

out vec2 vTexCoord;  

void main()
{
	vTexCoord = texcoord;  

	//calculate new vert positions
	vec4 pos = position;
	pos.y += texture(displacement, vec2(vTexCoord.x,vTexCoord.y + (Time * 0.03f))).r * 2;


	viewspace_position = view * vec4(pos.xyz, 1);
	viewspace_normal = view * vec4(0, 1, 0, 0); 

	gl_Position = view_proj * pos;
}