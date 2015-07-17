#version 410  

in vec2 vTexCoord;  
in vec4 viewspace_position;
in vec4 viewspace_normal;

layout(location=0) out vec3 albedo;
layout(location=1) out vec3 position;
layout(location=2) out vec3 normal;

uniform sampler2D perlin_texture;	

uniform sampler2D texture_stone;
uniform sampler2D texture_grass;

void main() 
{     

	position = viewspace_position.xyz;
	normal = (viewspace_normal.xyz) * 0.5f + 0.5f;

	vec4 grass_clr = texture(texture_grass, vTexCoord);
	vec4 stone_clr = texture(texture_stone, vTexCoord);
	float perlin = texture(perlin_texture, vTexCoord).r;


	perlin = clamp(perlin,0,1);


	//blend textures based on height
	albedo = mix(grass_clr.xyz, stone_clr.xyz, perlin).xyz;

	//albedo = normal.xyz;//texture(perlin_texture, vTexCoord).rrr; 


}