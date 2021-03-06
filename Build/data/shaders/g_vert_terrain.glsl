
#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;

out vec4 viewspace_position;
out vec4 viewspace_normal;

uniform mat4 view;
uniform mat4 view_proj;

uniform sampler2D perlin_texture;

uniform float max_height;

out vec2 vTexCoord;  

void main()
{
	vTexCoord = texcoord;  

	//calculate new vert positions
	vec4 pos = position;
	pos.y -= max_height;
	pos.y += texture(perlin_texture, vec2(vTexCoord.x,vTexCoord.y)).r * max_height;

	vec2 newTex = vec2(vTexCoord.x,vTexCoord.y);


	//normal

	int offsetPos = 10;
	float texturesize = textureSize( perlin_texture, 0 ).x;
	float textureoffset = 1 / texturesize;
	float sampleSize = 1.0f;

	vec4 posUp = pos;
	posUp.x += 0;
	posUp.y = (texture(perlin_texture, newTex + vec2( 0, textureoffset).r) * offsetPos).r;
	posUp.z += sampleSize;
	
	vec4 posUpRight = pos;
	posUpRight.x += sampleSize;
	posUpRight.y = (texture(perlin_texture, newTex + vec2( textureoffset, textureoffset).r) * offsetPos).r;
	posUpRight.z += sampleSize;

	vec4 posRight = pos;
	posRight.x += sampleSize;
	posRight.y = (texture(perlin_texture, newTex + vec2( textureoffset, 0).r) * offsetPos).r;

	vec4 posDownRight = pos;
	posDownRight.x += sampleSize;
	posDownRight.y = (texture(perlin_texture, newTex + vec2( textureoffset, -textureoffset ).r) * offsetPos).r;
	posDownRight.z -= sampleSize;

	vec4 posDown = pos;
	posDown.x += 0;
	posDown.y = (texture(perlin_texture, newTex + vec2( 0, -textureoffset ).r) * offsetPos).r;
	posDown.z -= sampleSize;

	vec4 posDownLeft = pos;
	posDownLeft.x -= sampleSize;
	posDownLeft.y = (texture(perlin_texture, newTex + vec2( -textureoffset, -textureoffset ).r) * offsetPos).r;
	posDownLeft.z -= sampleSize;

	vec4 posLeft = pos;
	posLeft.x -= sampleSize;
	posLeft.y = (texture(perlin_texture, newTex + vec2( -textureoffset, 0 ).r) * offsetPos).r;
	posLeft.z += 0;

	vec4 posUpLeft = pos;
	posUpLeft.x -= sampleSize;
	posUpLeft.y = (texture(perlin_texture, newTex + vec2( -textureoffset, textureoffset ).r) * offsetPos).r;
	posUpLeft.z += sampleSize;

	vec4 finalNormal = vec4(0,0,0,0);
	//


	finalNormal.xyz += cross( posUp.xyz - pos.xyz, posUpRight.xyz - pos.xyz);
	finalNormal.xyz += cross( posUpRight.xyz - pos.xyz, posRight.xyz - pos.xyz);
	finalNormal.xyz += cross( posRight.xyz - pos.xyz, posDownRight.xyz - pos.xyz);
	finalNormal.xyz += cross( posDownRight.xyz - pos.xyz, posDown.xyz - pos.xyz);

	finalNormal.xyz += cross( posDown.xyz - pos.xyz, posDownLeft.xyz - pos.xyz);
	finalNormal.xyz += cross( posDownLeft.xyz - pos.xyz, posLeft.xyz - pos.xyz);
	finalNormal.xyz += cross( posLeft.xyz - pos.xyz, posUpLeft.xyz - pos.xyz);
	finalNormal.xyz += cross( posUpLeft.xyz - pos.xyz, posUp.xyz - pos.xyz);


	vec3 normals = normalize( finalNormal.xyz );



	viewspace_position = view * vec4(pos.xyz, 1);
	viewspace_normal = view * vec4(normals.xyz, 0); 

	gl_Position = view_proj * pos;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////


//#version 410  
//
//layout(location=0) in vec4 position;
//layout(location=1) in vec2 texcoord;  
//
//uniform mat4 view;
//uniform mat4 view_proj;
//
//uniform sampler2D perlin_texture; 
//
//uniform float max_height;
//
//out vec4 viewspace_position;
//out vec4 viewspace_normal;
//out vec2 vTexCoord; 
//
//void main() 
//{     
//	vTexCoord = texcoord;    
//
//
//	//position
//	vec4 pos = position;
//	//pos.y -= max_height;
//	pos.y += texture(perlin_texture, texcoord).r * max_height;
//
//
//	//normal
//
//	int offsetPos = 50;
//	float texturesize = textureSize( perlin_texture, 0 ).x;
//	float textureoffset = 1 / texturesize;
//	float sampleSize = 1.0f;
//
//	vec4 posUp = pos;
//	posUp.x += 0;
//	posUp.y = (texture(perlin_texture, texcoord + vec2( 0, textureoffset).r) * offsetPos).r;
//	posUp.z += sampleSize;
//	
//	vec4 posUpRight = pos;
//	posUpRight.x += sampleSize;
//	posUpRight.y = (texture(perlin_texture, texcoord + vec2( textureoffset, textureoffset).r) * offsetPos).r;
//	posUpRight.z += sampleSize;
//
//	vec4 posRight = pos;
//	posRight.x += sampleSize;
//	posRight.y = (texture(perlin_texture, texcoord + vec2( textureoffset, 0).r) * offsetPos).r;
//
//	vec4 posDownRight = pos;
//	posDownRight.x += sampleSize;
//	posDownRight.y = (texture(perlin_texture, texcoord + vec2( textureoffset, -textureoffset ).r) * offsetPos).r;
//	posDownRight.z -= sampleSize;
//
//	vec4 posDown = pos;
//	posDown.x += 0;
//	posDown.y = (texture(perlin_texture, texcoord + vec2( 0, -textureoffset ).r) * offsetPos).r;
//	posDown.z -= sampleSize;
//
//	vec4 posDownLeft = pos;
//	posDownLeft.x -= sampleSize;
//	posDownLeft.y = (texture(perlin_texture, texcoord + vec2( -textureoffset, -textureoffset ).r) * offsetPos).r;
//	posDownLeft.z -= sampleSize;
//
//	vec4 posLeft = pos;
//	posLeft.x -= sampleSize;
//	posLeft.y = (texture(perlin_texture, texcoord + vec2( -textureoffset, 0 ).r) * offsetPos).r;
//	posLeft.z += 0;
//
//	vec4 posUpLeft = pos;
//	posUpLeft.x -= sampleSize;
//	posUpLeft.y = (texture(perlin_texture, texcoord + vec2( -textureoffset, textureoffset ).r) * offsetPos).r;
//	posUpLeft.z += sampleSize;
//
//	vec4 finalNormal = vec4(0,0,0,0);
//	//
//
//
//	finalNormal.xyz += cross( posUp.xyz - pos.xyz, posUpRight.xyz - pos.xyz);
//	finalNormal.xyz += cross( posUpRight.xyz - pos.xyz, posRight.xyz - pos.xyz);
//	finalNormal.xyz += cross( posRight.xyz - pos.xyz, posDownRight.xyz - pos.xyz);
//	finalNormal.xyz += cross( posDownRight.xyz - pos.xyz, posDown.xyz - pos.xyz);
//
//	finalNormal.xyz += cross( posDown.xyz - pos.xyz, posDownLeft.xyz - pos.xyz);
//	finalNormal.xyz += cross( posDownLeft.xyz - pos.xyz, posLeft.xyz - pos.xyz);
//	finalNormal.xyz += cross( posLeft.xyz - pos.xyz, posUpLeft.xyz - pos.xyz);
//	finalNormal.xyz += cross( posUpLeft.xyz - pos.xyz, posUp.xyz - pos.xyz);
//
//
//	vec3 normals = normalize( finalNormal.xyz );
//
//
//
//	
//	viewspace_position = view * vec4(pos.xyz, 1);
//	viewspace_normal = view * vec4(normals.xyz,1); 
//	 
//	gl_Position = view_proj * pos; 
//}