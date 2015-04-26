#version 410

in vec2 vTexCoord;
in vec4 viewspace_position;
in vec4 viewspace_normal;

layout(location=0) out vec3 albedo;
layout(location=1) out vec3 position;
layout(location=2) out vec3 normal;

uniform sampler2D diffuse;

void main()
{
	position = viewspace_position.xyz;
	normal = (viewspace_normal.xyz) * 0.5f + 0.5f;

	albedo = texture(diffuse, vTexCoord).xyz;
}
